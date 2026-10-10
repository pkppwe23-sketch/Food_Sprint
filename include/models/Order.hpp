#pragma once

#include "OrderItem.hpp"
#include "OrderStatus.hpp"
#include "Payment.hpp"
#include "Location.hpp"
#include <string>
#include <vector>
#include <memory>

namespace FoodSprint {

struct PriceBreakdown {
    double itemTotal = 0.0;
    double tax = 0.0;
    double deliveryFee = 0.0;
    double discount = 0.0;
    double finalAmount = 0.0;
    std::string couponCodeApplied;

    std::string toString() const;
};

class Order {
private:
    std::string id;
    std::string customerId;
    std::string restaurantId;
    std::string deliveryPartnerId;
    std::vector<OrderItem> items;
    Location deliveryLocation;
    PriceBreakdown priceBreakdown;
    OrderStatus status;
    std::shared_ptr<Payment> payment;
    std::vector<std::pair<OrderStatus, std::string>> statusHistory; // status + timestamp/notes
    std::string createdAt;

public:
    Order(std::string id, std::string customerId, std::string restaurantId,
          std::vector<OrderItem> items, Location deliveryLocation,
          PriceBreakdown priceBreakdown, std::string createdAt = "");

    const std::string& getId() const { return id; }
    const std::string& getCustomerId() const { return customerId; }
    const std::string& getRestaurantId() const { return restaurantId; }
    const std::string& getDeliveryPartnerId() const { return deliveryPartnerId; }
    const std::vector<OrderItem>& getItems() const { return items; }
    const Location& getDeliveryLocation() const { return deliveryLocation; }
    const PriceBreakdown& getPriceBreakdown() const { return priceBreakdown; }
    OrderStatus getStatus() const { return status; }
    std::shared_ptr<Payment> getPayment() const { return payment; }
    const std::vector<std::pair<OrderStatus, std::string>>& getStatusHistory() const { return statusHistory; }
    const std::string& getCreatedAt() const { return createdAt; }

    void setDeliveryPartnerId(const std::string& partnerId) { deliveryPartnerId = partnerId; }
    void setPayment(std::shared_ptr<Payment> pay) { payment = pay; }
    void setPriceBreakdown(const PriceBreakdown& pb) { priceBreakdown = pb; }

    bool updateStatus(OrderStatus newStatus, const std::string& note = "");

    std::string toString() const;
};

} // namespace FoodSprint
