#pragma once

#include "models/Order.hpp"
#include <string>

namespace FoodSprint {

class IOrderObserver {
public:
    virtual ~IOrderObserver() = default;
    virtual void onOrderStatusUpdated(const Order& order, OrderStatus oldStatus, OrderStatus newStatus) = 0;
    virtual std::string getObserverName() const = 0;
};

class CustomerNotificationObserver : public IOrderObserver {
public:
    void onOrderStatusUpdated(const Order& order, OrderStatus oldStatus, OrderStatus newStatus) override;
    std::string getObserverName() const override { return "CustomerNotificationObserver"; }
};

class RestaurantNotificationObserver : public IOrderObserver {
public:
    void onOrderStatusUpdated(const Order& order, OrderStatus oldStatus, OrderStatus newStatus) override;
    std::string getObserverName() const override { return "RestaurantNotificationObserver"; }
};

class DeliveryPartnerNotificationObserver : public IOrderObserver {
public:
    void onOrderStatusUpdated(const Order& order, OrderStatus oldStatus, OrderStatus newStatus) override;
    std::string getObserverName() const override { return "DeliveryPartnerNotificationObserver"; }
};

} // namespace FoodSprint
