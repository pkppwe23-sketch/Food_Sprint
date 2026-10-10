#include "models/Coupon.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace FoodSprint {

Coupon::Coupon(std::string code, DiscountType type, double discountValue,
               double minOrderValue, double maxDiscount, bool isActive)
    : code(std::move(code)), type(type), discountValue(discountValue),
      minOrderValue(minOrderValue), maxDiscountAmount(maxDiscount),
      isActive(isActive) {}

bool Coupon::isValid(double orderAmount) const {
    if (!isActive) return false;
    if (orderAmount < minOrderValue) return false;
    return true;
}

double Coupon::calculateDiscount(double orderAmount) const {
    if (!isValid(orderAmount)) return 0.0;

    double calculated = 0.0;
    if (type == DiscountType::PERCENTAGE) {
        calculated = (orderAmount * discountValue) / 100.0;
        calculated = std::min(calculated, maxDiscountAmount);
    } else { // FLAT
        calculated = std::min(discountValue, orderAmount);
    }
    return calculated;
}

std::string Coupon::toString() const {
    std::ostringstream oss;
    oss << "Coupon [" << code << "] "
        << (type == DiscountType::PERCENTAGE ? std::to_string((int)discountValue) + "% OFF" : "$" + std::to_string((int)discountValue) + " FLAT OFF")
        << " (Min Order: $" << std::fixed << std::setprecision(2) << minOrderValue
        << ", Max Cap: $" << maxDiscountAmount << ")"
        << (isActive ? " [Active]" : " [Inactive]");
    return oss.str();
}

} // namespace FoodSprint
