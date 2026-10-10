#pragma once

#include "Location.hpp"
#include <string>

namespace FoodSprint {

class DeliveryPartner {
private:
    std::string id;
    std::string name;
    std::string phone;
    std::string vehicleType; // e.g., "Bike", "Electric Scooter", "Car"
    Location currentLocation;
    bool isAvailable;
    double rating;
    int ratingCount;
    std::string currentOrderId;

public:
    DeliveryPartner(std::string id, std::string name, std::string phone,
                    std::string vehicleType, Location currentLocation);

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getPhone() const { return phone; }
    const std::string& getVehicleType() const { return vehicleType; }
    const Location& getCurrentLocation() const { return currentLocation; }
    bool getIsAvailable() const { return isAvailable; }
    double getRating() const { return rating; }
    int getRatingCount() const { return ratingCount; }
    const std::string& getCurrentOrderId() const { return currentOrderId; }

    void updateLocation(const Location& loc) { currentLocation = loc; }
    void setAvailable(bool available) { isAvailable = available; }
    void assignOrder(const std::string& orderId);
    void completeOrder();
    void addRating(double newRating);

    std::string toString() const;
};

} // namespace FoodSprint
