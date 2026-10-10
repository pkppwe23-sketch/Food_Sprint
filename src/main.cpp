#include "models/Location.hpp"
#include "models/DietaryPreference.hpp"
#include "models/MenuItem.hpp"
#include "models/Restaurant.hpp"
#include "models/User.hpp"
#include "models/DeliveryPartner.hpp"
#include "models/Order.hpp"
#include "models/Coupon.hpp"
#include "models/Review.hpp"

#include "strategies/IDeliveryFeeStrategy.hpp"
#include "strategies/IPartnerAssignmentStrategy.hpp"
#include "strategies/IPaymentStrategy.hpp"

#include "observers/IOrderObserver.hpp"

#include "repositories/InMemoryUserRepository.hpp"
#include "repositories/InMemoryRestaurantRepository.hpp"
#include "repositories/InMemoryDeliveryPartnerRepository.hpp"
#include "repositories/InMemoryOrderRepository.hpp"
#include "repositories/InMemoryCouponRepository.hpp"

#include "services/PricingService.hpp"
#include "services/FoodDeliveryService.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>

using namespace FoodSprint;

void printSectionHeader(const std::string& title) {
    std::cout << "\n======================================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "======================================================================\n";
}

void printSubHeader(const std::string& title) {
    std::cout << "\n>>> " << title << " <<<\n";
}

void seedInitialData(std::shared_ptr<InMemoryRestaurantRepository> rRepo,
                     std::shared_ptr<InMemoryUserRepository> uRepo,
                     std::shared_ptr<InMemoryDeliveryPartnerRepository> pRepo,
                     std::shared_ptr<InMemoryCouponRepository> cRepo) {

    // 1. Seed Users
    auto alice = std::make_shared<User>(
        "USR-001", "Alice Johnson", "alice@example.com", "+1-555-0101",
        Location(40.7128, -74.0060, "123 Manhattan Ave, NYC"));
    auto bob = std::make_shared<User>(
        "USR-002", "Bob Miller", "bob@example.com", "+1-555-0102",
        Location(40.7306, -73.9352, "456 Brooklyn Blvd, NYC"));
    auto charlie = std::make_shared<User>(
        "USR-003", "Charlie Davis", "charlie@example.com", "+1-555-0103",
        Location(40.7589, -73.9851, "789 Times Square, NYC"));

    uRepo->save(alice);
    uRepo->save(bob);
    uRepo->save(charlie);

    // 2. Seed Restaurants
    // Restaurant 1: Bella Italia
    auto r1 = std::make_shared<Restaurant>(
        "REST-001", "Bella Italia Trattoria", "Italian",
        Location(40.7200, -74.0000, "10 Little Italy St, NYC"));
    r1->addMenuItem(std::make_shared<MenuItem>(
        "IT-101", "Margherita Pizza", "San Marzano tomatoes, fresh mozzarella, basil",
        14.99, DietaryPreference::VEG, "Pizza"));
    r1->addMenuItem(std::make_shared<MenuItem>(
        "IT-102", "Pepperoni Feast Pizza", "Crispy pepperoni, spicy marinara, provolone",
        17.50, DietaryPreference::NON_VEG, "Pizza"));
    r1->addMenuItem(std::make_shared<MenuItem>(
        "IT-103", "Truffle Tagliatelle", "Wild mushrooms, black truffle butter, parmesan",
        21.00, DietaryPreference::VEG, "Pasta"));
    r1->addMenuItem(std::make_shared<MenuItem>(
        "IT-104", "Classic Tiramisu", "Espresso-soaked ladyfingers with mascarpone cream",
        8.50, DietaryPreference::VEG, "Desserts"));

    // Restaurant 2: The Spice Route
    auto r2 = std::make_shared<Restaurant>(
        "REST-002", "The Spice Route", "Indian",
        Location(40.7400, -73.9900, "55 Curry Hill, NYC"));
    r2->addMenuItem(std::make_shared<MenuItem>(
        "IN-201", "Paneer Butter Masala", "Cottage cheese simmered in buttery tomato gravy",
        15.00, DietaryPreference::VEG, "Curry"));
    r2->addMenuItem(std::make_shared<MenuItem>(
        "IN-202", "Chicken Dum Biryani", "Fragrant basmati rice slow-cooked with tender spiced chicken",
        18.50, DietaryPreference::NON_VEG, "Biryani"));
    r2->addMenuItem(std::make_shared<MenuItem>(
        "IN-203", "Vegan Dal Tadka", "Yellow lentils tempered with cumin, garlic & chili",
        12.00, DietaryPreference::VEGAN, "Lentils"));
    r2->addMenuItem(std::make_shared<MenuItem>(
        "IN-204", "Garlic Naan", "Tandoor baked flatbread with roasted garlic and cilantro",
        3.99, DietaryPreference::VEG, "Breads"));

    // Restaurant 3: Burger & Shake Co.
    auto r3 = std::make_shared<Restaurant>(
        "REST-003", "Burger & Shake Co.", "American",
        Location(40.7500, -73.9800, "99 Broadway Ave, NYC"));
    r3->addMenuItem(std::make_shared<MenuItem>(
        "BG-301", "Double Bacon Cheeseburger", "Angus beef patties, smoked cheddar, bacon",
        16.00, DietaryPreference::NON_VEG, "Burgers"));
    r3->addMenuItem(std::make_shared<MenuItem>(
        "BG-302", "Plant-Based Beyond Burger", "100% vegan patty, dairy-free cheddar, avocado",
        15.50, DietaryPreference::VEGAN, "Burgers"));
    r3->addMenuItem(std::make_shared<MenuItem>(
        "BG-303", "Seasoned Curly Fries", "Crispy seasoned spirals with spicy aioli",
        5.50, DietaryPreference::VEGAN, "Sides"));

    rRepo->save(r1);
    rRepo->save(r2);
    rRepo->save(r3);

    // 3. Seed Delivery Partners
    auto rider1 = std::make_shared<DeliveryPartner>(
        "RIDER-101", "Dave Evans", "+1-555-0201", "E-Bike",
        Location(40.7180, -74.0010, "SoHo Stn, NYC")); // Near Bella Italia
    rider1->addRating(4.8);

    auto rider2 = std::make_shared<DeliveryPartner>(
        "RIDER-102", "Emma Watson", "+1-555-0202", "Electric Scooter",
        Location(40.7380, -73.9910, "Union Sq, NYC")); // Near Spice Route
    rider2->addRating(4.95);

    auto rider3 = std::make_shared<DeliveryPartner>(
        "RIDER-103", "Frank Castle", "+1-555-0203", "Motorcycle",
        Location(40.7480, -73.9820, "Herald Sq, NYC")); // Near Burger Co.
    rider3->addRating(4.6);

    pRepo->save(rider1);
    pRepo->save(rider2);
    pRepo->save(rider3);

    // 4. Seed Coupons
    auto c1 = std::make_shared<Coupon>("WELCOME50", DiscountType::PERCENTAGE, 50.0, 20.0, 10.0);
    auto c2 = std::make_shared<Coupon>("FLAT5", DiscountType::FLAT, 5.0, 25.0);
    auto c3 = std::make_shared<Coupon>("GOURMET20", DiscountType::PERCENTAGE, 20.0, 35.0, 15.0);

    cRepo->save(c1);
    cRepo->save(c2);
    cRepo->save(c3);
}

void runAutomatedDemo() {
    printSectionHeader("FOOD SPRINT - LOW LEVEL DESIGN SYSTEM ARCHITECTURE DEMO");

    // Initialize dependencies (Inversion of Control & Dependency Injection)
    auto userRepo = std::make_shared<InMemoryUserRepository>();
    auto restaurantRepo = std::make_shared<InMemoryRestaurantRepository>();
    auto partnerRepo = std::make_shared<InMemoryDeliveryPartnerRepository>();
    auto orderRepo = std::make_shared<InMemoryOrderRepository>();
    auto couponRepo = std::make_shared<InMemoryCouponRepository>();

    auto feeStrategy = std::make_shared<DistanceBasedDeliveryFeeStrategy>(2.5, 0.75, 2.0);
    auto pricingService = std::make_shared<PricingService>(feeStrategy, couponRepo, 0.05);
    auto partnerAssignmentStrategy = std::make_shared<NearestPartnerAssignmentStrategy>();

    FoodDeliveryService foodService(
        userRepo, restaurantRepo, partnerRepo, orderRepo, couponRepo,
        pricingService, partnerAssignmentStrategy);

    // Register Observers (Observer Pattern)
    foodService.registerObserver(std::make_shared<CustomerNotificationObserver>());
    foodService.registerObserver(std::make_shared<RestaurantNotificationObserver>());
    foodService.registerObserver(std::make_shared<DeliveryPartnerNotificationObserver>());

    // Seed mock data
    seedInitialData(restaurantRepo, userRepo, partnerRepo, couponRepo);

    // =========================================================================
    // SCENARIO 1: Restaurant Discovery & Dietary Preference Filtering
    // =========================================================================
    printSectionHeader("1. RESTAURANT DISCOVERY & DIETARY FILTERING");
    std::cout << "Searching for Italian restaurants...\n";
    auto searchResults = foodService.searchRestaurants("Italian");
    for (const auto& r : searchResults) {
        std::cout << "  • " << r->toString() << "\n";
    }

    std::cout << "\nFiltering Vegetarian & Vegan dishes from 'The Spice Route' (REST-002):\n";
    auto spiceRoute = foodService.getRestaurant("REST-002");
    if (spiceRoute) {
        auto vegItems = spiceRoute->getItemsByPreference(DietaryPreference::VEG);
        auto veganItems = spiceRoute->getItemsByPreference(DietaryPreference::VEGAN);
        std::cout << "  Vegetarian items:\n";
        for (const auto& item : vegItems) std::cout << "    - " << item->toString() << "\n";
        std::cout << "  Vegan items:\n";
        for (const auto& item : veganItems) std::cout << "    - " << item->toString() << "\n";
    }

    // =========================================================================
    // SCENARIO 2: Order Placement with Coupon, Pricing & Nearest Partner Dispatch
    // =========================================================================
    printSectionHeader("2. ORDER PLACEMENT, DYNAMIC PRICING & PROXIMITY DISPATCH");

    auto alice = userRepo->findById("USR-001");
    auto bellaItalia = restaurantRepo->findById("REST-001");

    std::vector<OrderItem> cart1;
    cart1.emplace_back(bellaItalia->getMenuItem("IT-101"), 1, "Extra crispy crust");
    cart1.emplace_back(bellaItalia->getMenuItem("IT-103"), 1, "Less pepper");
    cart1.emplace_back(bellaItalia->getMenuItem("IT-104"), 2, "");

    std::cout << "Customer: Alice Johnson (" << alice->getId() << ")\n";
    std::cout << "Ordering from: " << bellaItalia->getName() << "\n";
    std::cout << "Applying Promo Code: WELCOME50 (50% off up to $10, min $20)\n\n";

    auto order1 = foodService.placeOrder(
        alice->getId(), bellaItalia->getId(), cart1, alice->getDefaultAddress(), "WELCOME50");

    if (order1) {
        std::cout << order1->toString() << "\n\n";

        printSubHeader("Processing Payment via UPI Strategy");
        UpiPaymentStrategy upiStrategy("alice@okhdfcbank");
        bool paymentSuccess = foodService.processOrderPayment(order1->getId(), upiStrategy);
        std::cout << "Payment Result: " << (paymentSuccess ? "SUCCESS" : "FAILED") << "\n\n";

        printSubHeader("Dispatching Nearest Available Courier");
        foodService.assignDeliveryPartner(order1->getId());

        printSubHeader("Order Status Lifecycle Transitions & Real-time Observer Broadcasts");
        foodService.startPreparingOrder(order1->getId());
        foodService.markOrderReady(order1->getId());
        foodService.pickUpOrder(order1->getId());
        foodService.completeDelivery(order1->getId());

        printSubHeader("Customer Rating & Feedback Submission");
        foodService.submitReview(order1->getId(), 5.0, 5.0, "The truffle pasta was incredible and courier arrived super hot!");

        // Display updated restaurant and courier ratings
        std::cout << "Updated Restaurant: " << bellaItalia->toString() << "\n";
        auto rider1 = partnerRepo->findById("RIDER-101");
        if (rider1) {
            std::cout << "Updated Courier: " << rider1->toString() << "\n";
        }
    }

    // =========================================================================
    // SCENARIO 3: Surge Pricing / Peak Weather Simulation (Strategy Pattern Switch)
    // =========================================================================
    printSectionHeader("3. SURGE PRICING ACTIVATION (STRATEGY PATTERN IN ACTION)");

    std::cout << "🌧️ Heavy monsoon rainfall detected! Switching delivery fee strategy to Surge Pricing...\n";
    auto surgeStrategy = std::make_shared<SurgeDeliveryFeeStrategy>(2.0, "Heavy Monsoon Downpour", 3.0, 1.2);
    foodService.setDeliveryFeeStrategy(surgeStrategy);
    std::cout << "Active Delivery Strategy: " << surgeStrategy->getStrategyName() << "\n\n";

    auto bob = userRepo->findById("USR-002");
    auto burgerCo = restaurantRepo->findById("REST-003");

    std::vector<OrderItem> cart2;
    cart2.emplace_back(burgerCo->getMenuItem("BG-301"), 2, "Well done");
    cart2.emplace_back(burgerCo->getMenuItem("BG-303"), 1, "Extra dip");

    auto order2 = foodService.placeOrder(
        bob->getId(), burgerCo->getId(), cart2, bob->getDefaultAddress());

    if (order2) {
        std::cout << order2->toString() << "\n\n";

        printSubHeader("Processing Payment via Credit Card Strategy");
        CreditCardPaymentStrategy cardStrategy("4111222233334444", "Bob Miller", "888", "12/28");
        foodService.processOrderPayment(order2->getId(), cardStrategy);

        printSubHeader("Courier Dispatch & Delivery Completion");
        foodService.assignDeliveryPartner(order2->getId());
        foodService.startPreparingOrder(order2->getId());
        foodService.markOrderReady(order2->getId());
        foodService.pickUpOrder(order2->getId());
        foodService.completeDelivery(order2->getId());
    }

    // =========================================================================
    // SCENARIO 4: Order Cancellation & Instant Wallet Refund Handling
    // =========================================================================
    printSectionHeader("4. ORDER CANCELLATION & INSTANT WALLET REFUND");

    auto charlie = userRepo->findById("USR-003");
    std::vector<OrderItem> cart3;
    cart3.emplace_back(spiceRoute->getMenuItem("IN-202"), 1, "Medium spice");

    std::cout << "Charlie orders Chicken Dum Biryani from The Spice Route...\n";
    auto order3 = foodService.placeOrder(
        charlie->getId(), spiceRoute->getId(), cart3, charlie->getDefaultAddress());

    if (order3) {
        printSubHeader("Payment with Digital Wallet (Preloaded: $50.00)");
        WalletPaymentStrategy walletStrategy("WALLET-CHARLIE-01", 50.0);
        foodService.processOrderPayment(order3->getId(), walletStrategy);

        std::cout << "\nOrder status before cancellation: " << orderStatusToString(order3->getStatus()) << "\n";
        printSubHeader("Customer cancels order before kitchen starts preparing");
        bool cancelOk = foodService.cancelOrder(order3->getId(), &walletStrategy, "Customer changed mind");
        std::cout << "Cancellation outcome: " << (cancelOk ? "SUCCESS (Refunded)" : "FAILED") << "\n";
        std::cout << "Order status after cancellation: " << orderStatusToString(order3->getStatus()) << "\n";
        std::cout << "Charlie's Wallet Balance after refund: $" << walletStrategy.getBalance() << "\n";
    }

    // =========================================================================
    // SCENARIO 5: Highest-Rated Courier Assignment Strategy
    // =========================================================================
    printSectionHeader("5. RATING-BASED COURIER ASSIGNMENT STRATEGY");

    std::cout << "Switching partner dispatch algorithm to: HighestRatedPartnerAssignmentStrategy (15km radius)\n";
    auto ratingAssignmentStrategy = std::make_shared<HighestRatedPartnerAssignmentStrategy>(15.0);
    foodService.setPartnerAssignmentStrategy(ratingAssignmentStrategy);
    std::cout << "Active Dispatch Strategy: " << ratingAssignmentStrategy->getStrategyName() << "\n\n";

    std::vector<OrderItem> cart4;
    cart4.emplace_back(bellaItalia->getMenuItem("IT-102"), 1, "");

    auto order4 = foodService.placeOrder(
        alice->getId(), bellaItalia->getId(), cart4, alice->getDefaultAddress());
    if (order4) {
        CashOnDeliveryStrategy codStrategy;
        foodService.processOrderPayment(order4->getId(), codStrategy);
        foodService.assignDeliveryPartner(order4->getId());
    }

    printSectionHeader("ALL FOOD SPRINT LLD ARCHITECTURE TESTS & FLOWS COMPLETED SUCCESSFULLY!");
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    runAutomatedDemo();
    return 0;
}
