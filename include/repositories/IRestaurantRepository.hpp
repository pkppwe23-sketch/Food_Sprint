#pragma once

#include "models/Restaurant.hpp"
#include <memory>
#include <vector>
#include <string>

namespace FoodSprint {

class IRestaurantRepository {
public:
    virtual ~IRestaurantRepository() = default;
    virtual void save(std::shared_ptr<Restaurant> restaurant) = 0;
    virtual std::shared_ptr<Restaurant> findById(const std::string& restaurantId) const = 0;
    virtual std::vector<std::shared_ptr<Restaurant>> findAll() const = 0;
    virtual std::vector<std::shared_ptr<Restaurant>> findByCuisine(const std::string& cuisine) const = 0;
    virtual std::vector<std::shared_ptr<Restaurant>> findOpenRestaurants() const = 0;
};

} // namespace FoodSprint
