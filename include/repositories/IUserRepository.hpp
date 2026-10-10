#pragma once

#include "models/User.hpp"
#include <memory>
#include <vector>
#include <string>

namespace FoodSprint {

class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void save(std::shared_ptr<User> user) = 0;
    virtual std::shared_ptr<User> findById(const std::string& userId) const = 0;
    virtual std::vector<std::shared_ptr<User>> findAll() const = 0;
};

} // namespace FoodSprint
