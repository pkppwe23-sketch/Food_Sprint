#pragma once

#include <string>

namespace FoodSprint {

enum class OrderStatus {
    PLACED,
    CONFIRMED,
    PREPARING,
    READY_FOR_PICKUP,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

inline std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PLACED:           return "PLACED";
        case OrderStatus::CONFIRMED:        return "CONFIRMED";
        case OrderStatus::PREPARING:        return "PREPARING";
        case OrderStatus::READY_FOR_PICKUP: return "READY_FOR_PICKUP";
        case OrderStatus::OUT_FOR_DELIVERY: return "OUT_FOR_DELIVERY";
        case OrderStatus::DELIVERED:        return "DELIVERED";
        case OrderStatus::CANCELLED:        return "CANCELLED";
        default:                            return "UNKNOWN";
    }
}

inline bool isValidStatusTransition(OrderStatus current, OrderStatus next) {
    if (current == OrderStatus::CANCELLED || current == OrderStatus::DELIVERED) {
        return false; // Terminal states
    }
    if (next == OrderStatus::CANCELLED) {
        // Can be cancelled only before out for delivery
        return current == OrderStatus::PLACED ||
               current == OrderStatus::CONFIRMED ||
               current == OrderStatus::PREPARING;
    }
    switch (current) {
        case OrderStatus::PLACED:
            return next == OrderStatus::CONFIRMED;
        case OrderStatus::CONFIRMED:
            return next == OrderStatus::PREPARING;
        case OrderStatus::PREPARING:
            return next == OrderStatus::READY_FOR_PICKUP;
        case OrderStatus::READY_FOR_PICKUP:
            return next == OrderStatus::OUT_FOR_DELIVERY;
        case OrderStatus::OUT_FOR_DELIVERY:
            return next == OrderStatus::DELIVERED;
        default:
            return false;
    }
}

} // namespace FoodSprint
