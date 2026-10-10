#pragma once

#include "Location.hpp"
#include "MenuItem.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace FoodSprint {

class Restaurant {
private:
    std::string id;
    std::string name;
    std::string cuisine;
    Location location;
    std::vector<std::shared_ptr<MenuItem>> menu;
    double rating;
    int ratingCount;
    bool isOpen;

public:
    Restaurant(std::string id, std::string name, std::string cuisine,
               Location location, bool isOpen = true);

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getCuisine() const { return cuisine; }
    const Location& getLocation() const { return location; }
    const std::vector<std::shared_ptr<MenuItem>>& getMenu() const { return menu; }
    double getRating() const { return rating; }
    int getRatingCount() const { return ratingCount; }
    bool getIsOpen() const { return isOpen; }

    void setIsOpen(bool open) { isOpen = open; }
    void addMenuItem(std::shared_ptr<MenuItem> item);
    bool removeMenuItem(const std::string& itemId);
    std::shared_ptr<MenuItem> getMenuItem(const std::string& itemId) const;

    void addRating(double newRating);
    std::vector<std::shared_ptr<MenuItem>> getAvailableItems() const;
    std::vector<std::shared_ptr<MenuItem>> getItemsByPreference(DietaryPreference pref) const;
    std::vector<std::shared_ptr<MenuItem>> getItemsByCategory(const std::string& category) const;

    std::string toString() const;
};

} // namespace FoodSprint
