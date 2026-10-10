#pragma once

#include "Location.hpp"
#include <string>
#include <vector>

namespace FoodSprint {

class User {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    Location defaultAddress;
    std::vector<Location> savedAddresses;

public:
    User(std::string id, std::string name, std::string email,
         std::string phone, Location defaultAddress);

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const Location& getDefaultAddress() const { return defaultAddress; }
    const std::vector<Location>& getSavedAddresses() const { return savedAddresses; }

    void setDefaultAddress(const Location& loc) { defaultAddress = loc; }
    void addAddress(const Location& loc) { savedAddresses.push_back(loc); }

    std::string toString() const;
};

} // namespace FoodSprint
