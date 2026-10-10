#include "models/Restaurant.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace FoodSprint {

Restaurant::Restaurant(std::string id, std::string name, std::string cuisine,
                       Location location, bool isOpen)
    : id(std::move(id)), name(std::move(name)), cuisine(std::move(cuisine)),
      location(std::move(location)), rating(5.0), ratingCount(1), isOpen(isOpen) {}

void Restaurant::addMenuItem(std::shared_ptr<MenuItem> item) {
    if (!item) return;
    // Overwrite existing if matching ID, otherwise push back
    for (auto& existing : menu) {
        if (existing->getId() == item->getId()) {
            existing = item;
            return;
        }
    }
    menu.push_back(item);
}

bool Restaurant::removeMenuItem(const std::string& itemId) {
    auto it = std::remove_if(menu.begin(), menu.end(),
        [&itemId](const std::shared_ptr<MenuItem>& item) {
            return item && item->getId() == itemId;
        });
    if (it != menu.end()) {
        menu.erase(it, menu.end());
        return true;
    }
    return false;
}

std::shared_ptr<MenuItem> Restaurant::getMenuItem(const std::string& itemId) const {
    for (const auto& item : menu) {
        if (item && item->getId() == itemId) {
            return item;
        }
    }
    return nullptr;
}

void Restaurant::addRating(double newRating) {
    if (newRating < 1.0) newRating = 1.0;
    if (newRating > 5.0) newRating = 5.0;
    double currentSum = rating * ratingCount;
    ratingCount++;
    rating = (currentSum + newRating) / ratingCount;
}

std::vector<std::shared_ptr<MenuItem>> Restaurant::getAvailableItems() const {
    std::vector<std::shared_ptr<MenuItem>> result;
    for (const auto& item : menu) {
        if (item && item->getIsAvailable()) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<std::shared_ptr<MenuItem>> Restaurant::getItemsByPreference(DietaryPreference pref) const {
    std::vector<std::shared_ptr<MenuItem>> result;
    for (const auto& item : menu) {
        if (item && item->getPreference() == pref) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<std::shared_ptr<MenuItem>> Restaurant::getItemsByCategory(const std::string& category) const {
    std::vector<std::shared_ptr<MenuItem>> result;
    for (const auto& item : menu) {
        if (item && item->getCategory() == category) {
            result.push_back(item);
        }
    }
    return result;
}

std::string Restaurant::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << name << " (" << cuisine << ") | Rating: "
        << std::fixed << std::setprecision(1) << rating << "★ (" << ratingCount << " reviews) | "
        << (isOpen ? "Open" : "Closed") << " | Location: " << location.getAddress();
    return oss.str();
}

} // namespace FoodSprint
