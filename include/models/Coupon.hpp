#pragma once

#include <string>

namespace FoodSprint {

enum class DiscountType {
    PERCENTAGE,
    FLAT
};

class Coupon {
private:
    std::string code;
    DiscountType type;
    double discountValue; // percentage (e.g. 20.0 for 20%) or flat amount (e.g. 50.0)
    double minOrderValue;
    double maxDiscountAmount;
    bool isActive;

public:
    Coupon(std::string code, DiscountType type, double discountValue,
           double minOrderValue = 0.0, double maxDiscount = 100000.0,
           bool isActive = true);

    const std::string& getCode() const { return code; }
    DiscountType getType() const { return type; }
    double getDiscountValue() const { return discountValue; }
    double getMinOrderValue() const { return minOrderValue; }
    double getMaxDiscountAmount() const { return maxDiscountAmount; }
    bool getIsActive() const { return isActive; }

    void setIsActive(bool active) { isActive = active; }

    bool isValid(double orderAmount) const;
    double calculateDiscount(double orderAmount) const;

    std::string toString() const;
};

} // namespace FoodSprint
