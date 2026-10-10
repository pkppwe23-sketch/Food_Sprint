#include "repositories/InMemoryCouponRepository.hpp"

namespace FoodSprint {

void InMemoryCouponRepository::save(std::shared_ptr<Coupon> coupon) {
    if (!coupon) return;
    std::lock_guard<std::mutex> lock(mtx);
    coupons[coupon->getCode()] = coupon;
}

std::shared_ptr<Coupon> InMemoryCouponRepository::findByCode(const std::string& code) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = coupons.find(code);
    if (it != coupons.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Coupon>> InMemoryCouponRepository::findAll() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<Coupon>> result;
    for (const auto& pair : coupons) {
        result.push_back(pair.second);
    }
    return result;
}

} // namespace FoodSprint
