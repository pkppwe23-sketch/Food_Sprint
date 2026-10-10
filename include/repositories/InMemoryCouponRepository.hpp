#pragma once

#include "ICouponRepository.hpp"
#include <unordered_map>
#include <mutex>

namespace FoodSprint {

class InMemoryCouponRepository : public ICouponRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<Coupon>> coupons;
    mutable std::mutex mtx;

public:
    void save(std::shared_ptr<Coupon> coupon) override;
    std::shared_ptr<Coupon> findByCode(const std::string& code) const override;
    std::vector<std::shared_ptr<Coupon>> findAll() const override;
};

} // namespace FoodSprint
