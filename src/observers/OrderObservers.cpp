#include "observers/IOrderObserver.hpp"
#include <iostream>

namespace FoodSprint {

void CustomerNotificationObserver::onOrderStatusUpdated(
    const Order& order, OrderStatus /*oldStatus*/, OrderStatus newStatus) {

    std::cout << "  🔔 [SMS/Push -> Customer " << order.getCustomerId() << "]: "
              << "Order #" << order.getId() << " status updated to -> "
              << orderStatusToString(newStatus);

    switch (newStatus) {
        case OrderStatus::CONFIRMED:
            std::cout << " (Restaurant has accepted your order!)";
            break;
        case OrderStatus::PREPARING:
            std::cout << " (Chef is preparing your delicious meal 🍳)";
            break;
        case OrderStatus::READY_FOR_PICKUP:
            std::cout << " (Food packed & ready for courier)";
            break;
        case OrderStatus::OUT_FOR_DELIVERY:
            std::cout << " (Rider is on the way to " << order.getDeliveryLocation().getAddress() << " 🛵)";
            break;
        case OrderStatus::DELIVERED:
            std::cout << " (Enjoy your meal! Please rate us ⭐)";
            break;
        case OrderStatus::CANCELLED:
            std::cout << " (Your order has been cancelled and refunded)";
            break;
        default:
            break;
    }
    std::cout << "\n";
}

void RestaurantNotificationObserver::onOrderStatusUpdated(
    const Order& order, OrderStatus /*oldStatus*/, OrderStatus newStatus) {

    if (newStatus == OrderStatus::CONFIRMED) {
        std::cout << "  🔔 [KDS -> Kitchen " << order.getRestaurantId() << "]: "
                  << "NEW ORDER #" << order.getId() << " accepted! Please start cooking "
                  << order.getItems().size() << " items.\n";
    } else if (newStatus == OrderStatus::READY_FOR_PICKUP) {
        std::cout << "  🔔 [KDS -> Kitchen " << order.getRestaurantId() << "]: "
                  << "Order #" << order.getId() << " moved to pickup counter.\n";
    }
}

void DeliveryPartnerNotificationObserver::onOrderStatusUpdated(
    const Order& order, OrderStatus /*oldStatus*/, OrderStatus newStatus) {

    if (!order.getDeliveryPartnerId().empty()) {
        if (newStatus == OrderStatus::READY_FOR_PICKUP) {
            std::cout << "  🔔 [App -> Rider " << order.getDeliveryPartnerId() << "]: "
                      << "Order #" << order.getId() << " is ready! Proceed to kitchen pickup.\n";
        } else if (newStatus == OrderStatus::OUT_FOR_DELIVERY) {
            std::cout << "  🔔 [App -> Rider " << order.getDeliveryPartnerId() << "]: "
                      << "Picked up Order #" << order.getId() << ". Deliver to: "
                      << order.getDeliveryLocation().getAddress() << "\n";
        } else if (newStatus == OrderStatus::DELIVERED) {
            std::cout << "  🔔 [App -> Rider " << order.getDeliveryPartnerId() << "]: "
                      << "Delivery completed! Payout credited to your wallet.\n";
        }
    }
}

} // namespace FoodSprint
