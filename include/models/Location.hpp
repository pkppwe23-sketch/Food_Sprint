#pragma once

#include <string>
#include <cmath>

namespace FoodSprint {

class Location {
private:
    double latitude;
    double longitude;
    std::string address;

public:
    Location() : latitude(0.0), longitude(0.0), address("") {}
    Location(double lat, double lon, std::string addr = "")
        : latitude(lat), longitude(lon), address(std::move(addr)) {}

    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    const std::string& getAddress() const { return address; }

    void setLatitude(double lat) { latitude = lat; }
    void setLongitude(double lon) { longitude = lon; }
    void setAddress(const std::string& addr) { address = addr; }

    // Computes approximate Euclidean / Haversine distance in kilometers
    double distanceTo(const Location& other) const {
        constexpr double R = 6371.0; // Earth's radius in km
        constexpr double PI = 3.14159265358979323846;
        double lat1 = latitude * PI / 180.0;
        double lon1 = longitude * PI / 180.0;
        double lat2 = other.latitude * PI / 180.0;
        double lon2 = other.longitude * PI / 180.0;

        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;

        double a = std::sin(dlat / 2.0) * std::sin(dlat / 2.0) +
                   std::cos(lat1) * std::cos(lat2) *
                   std::sin(dlon / 2.0) * std::sin(dlon / 2.0);
        double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
        return R * c;
    }

    std::string toString() const {
        if (!address.empty()) {
            return address + " (" + std::to_string(latitude) + ", " + std::to_string(longitude) + ")";
        }
        return "(" + std::to_string(latitude) + ", " + std::to_string(longitude) + ")";
    }
};

} // namespace FoodSprint
