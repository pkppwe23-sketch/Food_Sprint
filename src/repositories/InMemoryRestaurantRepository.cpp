#include "repositories/InMemoryRestaurantRepository.hpp"

namespace FoodSprint {

void InMemoryRestaurantRepository::save(std::shared_ptr<Restaurant> restaurant) {
    if (!restaurant) return;
    std::lock_guard<std::mutex> lock(mtx);
    restaurants[restaurant->getId()] = restaurant;
}

std::shared_ptr<Restaurant> InMemoryRestaurantRepository::findById(const std::string& restaurantId) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = restaurants.find(restaurantId);
    if (it != restaurants.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Restaurant>> InMemoryRestaurantRepository::findAll() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Restaurant>> result;
    for (const auto& pair : restaurants) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::shared_ptr<Restaurant>> InMemoryRestaurantRepository::findByCuisine(const std::string& cuisine) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Restaurant>> result;
    for (const auto& pair : restaurants) {
        if (pair.second && pair.second->getCuisine() == cuisine) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Restaurant>> InMemoryRestaurantRepository::findOpenRestaurants() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Restaurant>> result;
    for (const auto& pair : restaurants) {
        if (pair.second && pair.second->getIsOpen()) {
            result.push_back(pair.second);
        }
    }
    return result;
}

} // namespace FoodSprint
