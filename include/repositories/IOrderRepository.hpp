#pragma once

#include "models/Order.hpp"
#include <memory>
#include <vector>
#include <string>

namespace FoodSprint {

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual void save(std::shared_ptr<Order> order) = 0;
    virtual std::shared_ptr<Order> findById(const std::string& orderId) const = 0;
    virtual std::vector<std::shared_ptr<Order>> findByCustomer(const std::string& customerId) const = 0;
    virtual std::vector<std::shared_ptr<Order>> findByRestaurant(const std::string& restaurantId) const = 0;
    virtual std::vector<std::shared_ptr<Order>> findAll() const = 0;
};

} // namespace FoodSprint
