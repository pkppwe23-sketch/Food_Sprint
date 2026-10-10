#pragma once

#include "IRestaurantRepository.hpp"
#include <unordered_map>
#include <mutex>

namespace FoodSprint {

class InMemoryRestaurantRepository : public IRestaurantRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<Restaurant>> restaurants;
    mutable std::mutex mtx;

public:
    void save(std::shared_ptr<Restaurant> restaurant) override;
    std::shared_ptr<Restaurant> findById(const std::string& restaurantId) const override;
    std::vector<std::shared_ptr<Restaurant>> findAll() const override;
    std::vector<std::shared_ptr<Restaurant>> findByCuisine(const std::string& cuisine) const override;
    std::vector<std::shared_ptr<Restaurant>> findOpenRestaurants() const override;
};

} // namespace FoodSprint
