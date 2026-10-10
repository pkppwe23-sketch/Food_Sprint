#pragma once

#include "IUserRepository.hpp"
#include <unordered_map>
#include <mutex>

namespace FoodSprint {

class InMemoryUserRepository : public IUserRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    mutable std::mutex mtx;

public:
    void save(std::shared_ptr<User> user) override {
        if (!user) return;
        std::lock_guard<std::mutex> lock(mtx);
        users[user->getId()] = user;
    }

    std::shared_ptr<User> findById(const std::string& userId) const override {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = users.find(userId);
        if (it != users.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<User>> findAll() const override {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<std::shared_ptr<User>> result;
        for (const auto& pair : users) {
            result.push_back(pair.second);
        }
        return result;
    }
};

} // namespace FoodSprint
