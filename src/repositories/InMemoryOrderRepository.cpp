#include "repositories/InMemoryOrderRepository.hpp"

namespace FoodSprint {

void InMemoryOrderRepository::save(std::shared_ptr<Order> order) {
    if (!order) return;
    std::lock_guard<std::mutex> lock(mtx);
    orders[order->getId()] = order;
}

std::shared_ptr<Order> InMemoryOrderRepository::findById(const std::string& orderId) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = orders.find(orderId);
    if (it != orders.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Order>> InMemoryOrderRepository::findByCustomer(const std::string& customerId) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Order>> result;
    for (const auto& pair : orders) {
        if (pair.second && pair.second->getCustomerId() == customerId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Order>> InMemoryOrderRepository::findByRestaurant(const std::string& restaurantId) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Order>> result;
    for (const auto& pair : orders) {
        if (pair.second && pair.second->getRestaurantId() == restaurantId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Order>> InMemoryOrderRepository::findAll() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Order>> result;
    for (const auto& pair : orders) {
        result.push_back(pair.second);
    }
    return result;
}

} // namespace FoodSprint
