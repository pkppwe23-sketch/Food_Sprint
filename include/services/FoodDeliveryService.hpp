#pragma once

#include "models/User.hpp"
#include "models/Restaurant.hpp"
#include "models/DeliveryPartner.hpp"
#include "models/Order.hpp"
#include "models/Review.hpp"
#include "strategies/IDeliveryFeeStrategy.hpp"
#include "strategies/IPartnerAssignmentStrategy.hpp"
#include "strategies/IPaymentStrategy.hpp"
#include "observers/IOrderObserver.hpp"
#include "repositories/IUserRepository.hpp"
#include "repositories/IRestaurantRepository.hpp"
#include "repositories/IDeliveryPartnerRepository.hpp"
#include "repositories/IOrderRepository.hpp"
#include "repositories/ICouponRepository.hpp"
#include "services/PricingService.hpp"

#include <memory>
#include <vector>
#include <string>
#include <mutex>

namespace FoodSprint {

class FoodDeliveryService {
private:
    std::shared_ptr<IUserRepository> userRepo;
    std::shared_ptr<IRestaurantRepository> restaurantRepo;
    std::shared_ptr<IDeliveryPartnerRepository> partnerRepo;
    std::shared_ptr<IOrderRepository> orderRepo;
    std::shared_ptr<ICouponRepository> couponRepo;
    std::shared_ptr<PricingService> pricingService;
    std::shared_ptr<IPartnerAssignmentStrategy> partnerAssignmentStrategy;
    std::vector<std::shared_ptr<IOrderObserver>> observers;

    mutable std::mutex serviceMutex;
    int orderCounter;

    void notifyObservers(const Order& order, OrderStatus oldStatus, OrderStatus newStatus);
    std::string generateOrderId();

public:
    FoodDeliveryService(
        std::shared_ptr<IUserRepository> userRepo,
        std::shared_ptr<IRestaurantRepository> restaurantRepo,
        std::shared_ptr<IDeliveryPartnerRepository> partnerRepo,
        std::shared_ptr<IOrderRepository> orderRepo,
        std::shared_ptr<ICouponRepository> couponRepo,
        std::shared_ptr<PricingService> pricingService,
        std::shared_ptr<IPartnerAssignmentStrategy> partnerAssignmentStrategy);

    // Entity registration & retrieval
    void registerUser(std::shared_ptr<User> user);
    std::shared_ptr<User> getUser(const std::string& userId) const;

    void registerRestaurant(std::shared_ptr<Restaurant> restaurant);
    std::shared_ptr<Restaurant> getRestaurant(const std::string& restaurantId) const;
    std::vector<std::shared_ptr<Restaurant>> searchRestaurants(const std::string& query) const;
    std::vector<std::shared_ptr<Restaurant>> getNearbyRestaurants(const Location& location, double maxRadiusKm) const;

    void registerDeliveryPartner(std::shared_ptr<DeliveryPartner> partner);
    std::shared_ptr<DeliveryPartner> getDeliveryPartner(const std::string& partnerId) const;
    void updatePartnerLocation(const std::string& partnerId, const Location& newLocation);

    void addCoupon(std::shared_ptr<Coupon> coupon);

    // Strategy configuration
    void setDeliveryFeeStrategy(std::shared_ptr<IDeliveryFeeStrategy> strategy);
    void setPartnerAssignmentStrategy(std::shared_ptr<IPartnerAssignmentStrategy> strategy);
    std::shared_ptr<IPartnerAssignmentStrategy> getPartnerAssignmentStrategy() const {
        return partnerAssignmentStrategy;
    }

    // Observer management
    void registerObserver(std::shared_ptr<IOrderObserver> observer);
    void unregisterObserver(const std::string& observerName);

    // Order Lifecycle
    std::shared_ptr<Order> placeOrder(
        const std::string& customerId,
        const std::string& restaurantId,
        const std::vector<OrderItem>& items,
        const Location& deliveryLocation,
        const std::string& couponCode = "");

    bool processOrderPayment(const std::string& orderId, IPaymentStrategy& paymentStrategy);
    bool confirmOrder(const std::string& orderId);
    bool assignDeliveryPartner(const std::string& orderId);
    bool startPreparingOrder(const std::string& orderId);
    bool markOrderReady(const std::string& orderId);
    bool pickUpOrder(const std::string& orderId);
    bool completeDelivery(const std::string& orderId);
    bool cancelOrder(const std::string& orderId, IPaymentStrategy* paymentStrategy = nullptr, const std::string& reason = "User Request");

    // Feedback
    bool submitReview(const std::string& orderId, double restaurantRating, double deliveryRating, const std::string& comment);

    // Query orders
    std::shared_ptr<Order> getOrder(const std::string& orderId) const;
    std::vector<std::shared_ptr<Order>> getCustomerOrders(const std::string& customerId) const;
};

} // namespace FoodSprint
