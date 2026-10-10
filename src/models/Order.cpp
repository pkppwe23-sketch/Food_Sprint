#include "models/Order.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

std::string PriceBreakdown::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Items: $" << itemTotal
        << " + Tax(5%): $" << tax
        << " + Delivery: $" << deliveryFee;
    if (discount > 0.0) {
        oss << " - Discount (" << couponCodeApplied << "): $" << discount;
    }
    oss << " = Total: $" << finalAmount;
    return oss.str();
}

Order::Order(std::string id, std::string customerId, std::string restaurantId,
             std::vector<OrderItem> items, Location deliveryLocation,
             PriceBreakdown priceBreakdown, std::string createdAt)
    : id(std::move(id)), customerId(std::move(customerId)),
      restaurantId(std::move(restaurantId)), deliveryPartnerId(""),
      items(std::move(items)), deliveryLocation(std::move(deliveryLocation)),
      priceBreakdown(priceBreakdown), status(OrderStatus::PLACED),
      payment(nullptr), createdAt(std::move(createdAt)) {
    statusHistory.emplace_back(OrderStatus::PLACED, "Order created & placed");
}

bool Order::updateStatus(OrderStatus newStatus, const std::string& note) {
    if (isValidStatusTransition(status, newStatus)) {
        status = newStatus;
        std::string entry = orderStatusToString(newStatus);
        if (!note.empty()) {
            entry += " (" + note + ")";
        }
        statusHistory.emplace_back(newStatus, entry);
        return true;
    }
    return false;
}

std::string Order::toString() const {
    std::ostringstream oss;
    oss << "========================================\n"
        << "Order ID: #" << id << " | Status: " << orderStatusToString(status) << "\n"
        << "Customer: " << customerId << " | Restaurant: " << restaurantId << "\n";
    if (!deliveryPartnerId.empty()) {
        oss << "Delivery Partner: " << deliveryPartnerId << "\n";
    }
    oss << "Delivery Location: " << deliveryLocation.getAddress() << "\n"
        << "----------------------------------------\n"
        << "Items:\n";
    for (const auto& item : items) {
        oss << "  • " << item.toString() << "\n";
    }
    oss << "----------------------------------------\n"
        << "Price: " << priceBreakdown.toString() << "\n";
    if (payment) {
        oss << "Payment: " << payment->toString() << "\n";
    }
    oss << "========================================";
    return oss.str();
}

} // namespace FoodSprint
