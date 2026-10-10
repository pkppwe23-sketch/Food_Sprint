#pragma once

#include "IOrderRepository.hpp"
#include <unordered_map>
#include <mutex>

namespace FoodSprint {

class InMemoryOrderRepository : public IOrderRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<Order>> orders;
    mutable std::mutex mtx;

public:
    void save(std::shared_ptr<Order> order) override;
    std::shared_ptr<Order> findById(const std::string& orderId) const override;
    std::vector<std::shared_ptr<Order>> findByCustomer(const std::string& customerId) const override;
    std::vector<std::shared_ptr<Order>> findByRestaurant(const std::string& restaurantId) const override;
    std::vector<std::shared_ptr<Order>> findAll() const override;
};

} // namespace FoodSprint
