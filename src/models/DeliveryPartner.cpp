#include "models/DeliveryPartner.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

DeliveryPartner::DeliveryPartner(std::string id, std::string name, std::string phone,
                                 std::string vehicleType, Location currentLocation)
    : id(std::move(id)), name(std::move(name)), phone(std::move(phone)),
      vehicleType(std::move(vehicleType)), currentLocation(std::move(currentLocation)),
      isAvailable(true), rating(5.0), ratingCount(1), currentOrderId("") {}

void DeliveryPartner::assignOrder(const std::string& orderId) {
    currentOrderId = orderId;
    isAvailable = false;
}

void DeliveryPartner::completeOrder() {
    currentOrderId = "";
    isAvailable = true;
}

void DeliveryPartner::addRating(double newRating) {
    if (newRating < 1.0) newRating = 1.0;
    if (newRating > 5.0) newRating = 5.0;
    double currentSum = rating * ratingCount;
    ratingCount++;
    rating = (currentSum + newRating) / ratingCount;
}

std::string DeliveryPartner::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << name << " (" << vehicleType << ") | Rating: "
        << std::fixed << std::setprecision(1) << rating << "★ | Status: "
        << (isAvailable ? "Available" : "On Delivery (Order: " + currentOrderId + ")")
        << " | Location: " << currentLocation.getAddress();
    return oss.str();
}

} // namespace FoodSprint
