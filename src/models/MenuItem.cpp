#include "models/MenuItem.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

MenuItem::MenuItem(std::string id, std::string name, std::string description,
                   double price, DietaryPreference preference, std::string category,
                   bool isAvailable)
    : id(std::move(id)), name(std::move(name)), description(std::move(description)),
      price(price), preference(preference), category(std::move(category)),
      isAvailable(isAvailable) {}

std::string MenuItem::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << name << " (" << dietaryPreferenceToString(preference)
        << ", " << category << ") - $" << std::fixed << std::setprecision(2) << price
        << (isAvailable ? " [Available]" : " [Out of Stock]");
    return oss.str();
}

} // namespace FoodSprint
