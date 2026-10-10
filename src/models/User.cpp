#include "models/User.hpp"
#include <sstream>

namespace FoodSprint {

User::User(std::string id, std::string name, std::string email,
           std::string phone, Location defaultAddress)
    : id(std::move(id)), name(std::move(name)), email(std::move(email)),
      phone(std::move(phone)), defaultAddress(std::move(defaultAddress)) {
    savedAddresses.push_back(this->defaultAddress);
}

std::string User::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << name << " | Email: " << email
        << " | Phone: " << phone << " | Address: " << defaultAddress.toString();
    return oss.str();
}

} // namespace FoodSprint
