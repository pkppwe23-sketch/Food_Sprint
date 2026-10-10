#pragma once

#include "DietaryPreference.hpp"
#include <string>

namespace FoodSprint {

class MenuItem {
private:
    std::string id;
    std::string name;
    std::string description;
    double price;
    DietaryPreference preference;
    std::string category; // e.g., "Starters", "Main Course", "Desserts", "Beverages"
    bool isAvailable;

public:
    MenuItem(std::string id, std::string name, std::string description,
             double price, DietaryPreference preference, std::string category,
             bool isAvailable = true);

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    double getPrice() const { return price; }
    DietaryPreference getPreference() const { return preference; }
    const std::string& getCategory() const { return category; }
    bool getIsAvailable() const { return isAvailable; }

    void setPrice(double newPrice) { price = newPrice; }
    void setIsAvailable(bool available) { isAvailable = available; }
    void setDescription(const std::string& desc) { description = desc; }

    std::string toString() const;
};

} // namespace FoodSprint
