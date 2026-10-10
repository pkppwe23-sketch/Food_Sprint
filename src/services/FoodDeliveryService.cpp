#include "services/FoodDeliveryService.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace FoodSprint {

FoodDeliveryService::FoodDeliveryService(
    std::shared_ptr<IUserRepository> userRepo,
    std::shared_ptr<IRestaurantRepository> restaurantRepo,
    std::shared_ptr<IDeliveryPartnerRepository> partnerRepo,
    std::shared_ptr<IOrderRepository> orderRepo,
    std::shared_ptr<ICouponRepository> couponRepo,
    std::shared_ptr<PricingService> pricingService,
    std::shared_ptr<IPartnerAssignmentStrategy> partnerAssignmentStrategy)
    : userRepo(std::move(userRepo)),
      restaurantRepo(std::move(restaurantRepo)),
      partnerRepo(std::move(partnerRepo)),
      orderRepo(std::move(orderRepo)),
      couponRepo(std::move(couponRepo)),
      pricingService(std::move(pricingService)),
      partnerAssignmentStrategy(std::move(partnerAssignmentStrategy)),
      orderCounter(1000) {}

std::string FoodDeliveryService::generateOrderId() {
    return "ORD-" + std::to_string(++orderCounter);
}

void FoodDeliveryService::notifyObservers(const Order& order, OrderStatus oldStatus, OrderStatus newStatus) {
    for (const auto& obs : observers) {
        if (obs) {
            obs->onOrderStatusUpdated(order, oldStatus, newStatus);
        }
    }
}

void FoodDeliveryService::registerUser(std::shared_ptr<User> user) {
    if (userRepo && user) {
        userRepo->save(user);
    }
}

std::shared_ptr<User> FoodDeliveryService::getUser(const std::string& userId) const {
    return userRepo ? userRepo->findById(userId) : nullptr;
}

void FoodDeliveryService::registerRestaurant(std::shared_ptr<Restaurant> restaurant) {
    if (restaurantRepo && restaurant) {
        restaurantRepo->save(restaurant);
    }
}

std::shared_ptr<Restaurant> FoodDeliveryService::getRestaurant(const std::string& restaurantId) const {
    return restaurantRepo ? restaurantRepo->findById(restaurantId) : nullptr;
}

std::vector<std::shared_ptr<Restaurant>> FoodDeliveryService::searchRestaurants(const std::string& query) const {
    std::vector<std::shared_ptr<Restaurant>> results;
    if (!restaurantRepo) return results;

    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    auto all = restaurantRepo->findAll();
    for (const auto& r : all) {
        std::string nameLower = r->getName();
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        std::string cuisineLower = r->getCuisine();
        std::transform(cuisineLower.begin(), cuisineLower.end(), cuisineLower.begin(), ::tolower);

        if (nameLower.find(lowerQuery) != std::string::npos ||
            cuisineLower.find(lowerQuery) != std::string::npos) {
            results.push_back(r);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Restaurant>> FoodDeliveryService::getNearbyRestaurants(
    const Location& location, double maxRadiusKm) const {
    std::vector<std::shared_ptr<Restaurant>> nearby;
    if (!restaurantRepo) return nearby;

    auto all = restaurantRepo->findAll();
    for (const auto& r : all) {
        if (r && r->getIsOpen()) {
            double dist = location.distanceTo(r->getLocation());
            if (dist <= maxRadiusKm) {
                nearby.push_back(r);
            }
        }
    }
    return nearby;
}

void FoodDeliveryService::registerDeliveryPartner(std::shared_ptr<DeliveryPartner> partner) {
    if (partnerRepo && partner) {
        partnerRepo->save(partner);
    }
}

std::shared_ptr<DeliveryPartner> FoodDeliveryService::getDeliveryPartner(const std::string& partnerId) const {
    return partnerRepo ? partnerRepo->findById(partnerId) : nullptr;
}

void FoodDeliveryService::updatePartnerLocation(const std::string& partnerId, const Location& newLocation) {
    if (partnerRepo) {
        auto partner = partnerRepo->findById(partnerId);
        if (partner) {
            partner->updateLocation(newLocation);
        }
    }
}

void FoodDeliveryService::addCoupon(std::shared_ptr<Coupon> coupon) {
    if (couponRepo && coupon) {
        couponRepo->save(coupon);
    }
}

void FoodDeliveryService::setDeliveryFeeStrategy(std::shared_ptr<IDeliveryFeeStrategy> strategy) {
    if (pricingService) {
        pricingService->setDeliveryFeeStrategy(strategy);
    }
}

void FoodDeliveryService::setPartnerAssignmentStrategy(std::shared_ptr<IPartnerAssignmentStrategy> strategy) {
    if (strategy) {
        partnerAssignmentStrategy = strategy;
    }
}

void FoodDeliveryService::registerObserver(std::shared_ptr<IOrderObserver> observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void FoodDeliveryService::unregisterObserver(const std::string& observerName) {
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
            [&observerName](const std::shared_ptr<IOrderObserver>& obs) {
                return obs && obs->getObserverName() == observerName;
            }),
        observers.end());
}

std::shared_ptr<Order> FoodDeliveryService::placeOrder(
    const std::string& customerId,
    const std::string& restaurantId,
    const std::vector<OrderItem>& items,
    const Location& deliveryLocation,
    const std::string& couponCode) {

    std::lock_guard<std::mutex> lock(serviceMutex);

    auto customer = userRepo ? userRepo->findById(customerId) : nullptr;
    if (!customer) {
        std::cerr << "Error: Customer " << customerId << " not found!\n";
        return nullptr;
    }

    auto restaurant = restaurantRepo ? restaurantRepo->findById(restaurantId) : nullptr;
    if (!restaurant) {
        std::cerr << "Error: Restaurant " << restaurantId << " not found!\n";
        return nullptr;
    }

    if (!restaurant->getIsOpen()) {
        std::cerr << "Error: Restaurant " << restaurant->getName() << " is currently closed!\n";
        return nullptr;
    }

    if (items.empty()) {
        std::cerr << "Error: Cannot place empty order!\n";
        return nullptr;
    }

    // Calculate distance
    double distanceKm = restaurant->getLocation().distanceTo(deliveryLocation);

    // Calculate price
    PriceBreakdown pb = pricingService->calculatePrice(items, distanceKm, couponCode);

    std::string orderId = generateOrderId();
    auto order = std::make_shared<Order>(orderId, customerId, restaurantId,
                                         items, deliveryLocation, pb, "2026-10-10 12:00:00");

    if (orderRepo) {
        orderRepo->save(order);
    }

    notifyObservers(*order, OrderStatus::PLACED, OrderStatus::PLACED);
    return order;
}

bool FoodDeliveryService::processOrderPayment(const std::string& orderId, IPaymentStrategy& paymentStrategy) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    if (order->getStatus() != OrderStatus::PLACED) {
        std::cerr << "Error: Cannot process payment for order in status "
                  << orderStatusToString(order->getStatus()) << "\n";
        return false;
    }

    auto payment = paymentStrategy.processPayment(orderId, order->getPriceBreakdown().finalAmount);
    order->setPayment(payment);

    if (payment && (payment->getStatus() == PaymentStatus::SUCCESS ||
                    payment->getStatus() == PaymentStatus::PENDING)) {
        OrderStatus oldStatus = order->getStatus();
        if (order->updateStatus(OrderStatus::CONFIRMED, "Payment verified (" + paymentModeToString(payment->getMode()) + ")")) {
            notifyObservers(*order, oldStatus, OrderStatus::CONFIRMED);
            return true;
        }
    }
    return false;
}

bool FoodDeliveryService::confirmOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::CONFIRMED, "Manually confirmed")) {
        notifyObservers(*order, oldStatus, OrderStatus::CONFIRMED);
        return true;
    }
    return false;
}

bool FoodDeliveryService::assignDeliveryPartner(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    auto restaurant = restaurantRepo ? restaurantRepo->findById(order->getRestaurantId()) : nullptr;
    if (!restaurant) return false;

    auto available = partnerRepo ? partnerRepo->findAvailablePartners() : std::vector<std::shared_ptr<DeliveryPartner>>{};
    if (available.empty()) {
        std::cout << "  ⚠️ Warning: No delivery partners currently available in the area for Order #" << orderId << "!\n";
        return false;
    }

    auto assigned = partnerAssignmentStrategy->assignPartner(available, restaurant->getLocation());
    if (assigned) {
        assigned->assignOrder(orderId);
        order->setDeliveryPartnerId(assigned->getId());
        double dist = assigned->getCurrentLocation().distanceTo(restaurant->getLocation());
        std::cout << "  🛵 Partner Assigned: " << assigned->getName() << " (" << assigned->getVehicleType()
                  << ") | Rating: " << assigned->getRating() << "★ | Distance to kitchen: "
                  << std::fixed << std::setprecision(2) << dist << " km\n";
        return true;
    }
    return false;
}

bool FoodDeliveryService::startPreparingOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::PREPARING, "Chef started cooking")) {
        notifyObservers(*order, oldStatus, OrderStatus::PREPARING);
        return true;
    }
    return false;
}

bool FoodDeliveryService::markOrderReady(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::READY_FOR_PICKUP, "Packed & ready on pickup counter")) {
        notifyObservers(*order, oldStatus, OrderStatus::READY_FOR_PICKUP);
        return true;
    }
    return false;
}

bool FoodDeliveryService::pickUpOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::OUT_FOR_DELIVERY, "Picked up by rider")) {
        notifyObservers(*order, oldStatus, OrderStatus::OUT_FOR_DELIVERY);
        return true;
    }
    return false;
}

bool FoodDeliveryService::completeDelivery(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::DELIVERED, "Handed over to customer")) {
        // Free partner
        if (!order->getDeliveryPartnerId().empty() && partnerRepo) {
            auto partner = partnerRepo->findById(order->getDeliveryPartnerId());
            if (partner) {
                partner->completeOrder();
                partner->updateLocation(order->getDeliveryLocation());
            }
        }
        notifyObservers(*order, oldStatus, OrderStatus::DELIVERED);
        return true;
    }
    return false;
}

bool FoodDeliveryService::cancelOrder(const std::string& orderId, IPaymentStrategy* paymentStrategy, const std::string& reason) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    OrderStatus oldStatus = order->getStatus();
    if (order->updateStatus(OrderStatus::CANCELLED, reason)) {
        // Free partner if one was assigned
        if (!order->getDeliveryPartnerId().empty() && partnerRepo) {
            auto partner = partnerRepo->findById(order->getDeliveryPartnerId());
            if (partner) {
                partner->completeOrder();
            }
        }
        // Refund if payment was made
        if (order->getPayment() && paymentStrategy) {
            paymentStrategy->refundPayment(order->getPayment());
        }
        notifyObservers(*order, oldStatus, OrderStatus::CANCELLED);
        return true;
    }
    std::cerr << "Error: Order #" << orderId << " cannot be cancelled at status "
              << orderStatusToString(oldStatus) << "\n";
    return false;
}

bool FoodDeliveryService::submitReview(const std::string& orderId, double restaurantRating, double deliveryRating, const std::string& comment) {
    std::lock_guard<std::mutex> lock(serviceMutex);
    auto order = orderRepo ? orderRepo->findById(orderId) : nullptr;
    if (!order) return false;

    if (order->getStatus() != OrderStatus::DELIVERED) {
        std::cerr << "Error: Can only review delivered orders!\n";
        return false;
    }

    auto restaurant = restaurantRepo ? restaurantRepo->findById(order->getRestaurantId()) : nullptr;
    if (restaurant) {
        restaurant->addRating(restaurantRating);
    }

    if (!order->getDeliveryPartnerId().empty() && partnerRepo) {
        auto partner = partnerRepo->findById(order->getDeliveryPartnerId());
        if (partner) {
            partner->addRating(deliveryRating);
        }
    }

    Review rev("REV-" + orderId, orderId, order->getCustomerId(),
               order->getRestaurantId(), order->getDeliveryPartnerId(),
               restaurantRating, deliveryRating, comment);
    std::cout << "  ⭐ Review Recorded: " << rev.toString() << "\n";
    return true;
}

std::shared_ptr<Order> FoodDeliveryService::getOrder(const std::string& orderId) const {
    return orderRepo ? orderRepo->findById(orderId) : nullptr;
}

std::vector<std::shared_ptr<Order>> FoodDeliveryService::getCustomerOrders(const std::string& customerId) const {
    return orderRepo ? orderRepo->findByCustomer(customerId) : std::vector<std::shared_ptr<Order>>{};
}

} // namespace FoodSprint
