#pragma once

#include "models/Coupon.hpp"
#include <memory>
#include <vector>
#include <string>

namespace FoodSprint {

class ICouponRepository {
public:
    virtual ~ICouponRepository() = default;
    virtual void save(std::shared_ptr<Coupon> coupon) = 0;
    virtual std::shared_ptr<Coupon> findByCode(const std::string& code) const = 0;
    virtual std::vector<std::shared_ptr<Coupon>> findAll() const = 0;
};

} // namespace FoodSprint
