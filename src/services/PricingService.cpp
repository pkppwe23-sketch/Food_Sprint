#include "services/PricingService.hpp"
#include <algorithm>
#include <cmath>

namespace FoodSprint {

PricingService::PricingService(std::shared_ptr<IDeliveryFeeStrategy> feeStrategy,
                               std::shared_ptr<ICouponRepository> couponRepo,
                               double taxRate)
    : deliveryFeeStrategy(std::move(feeStrategy)),
      couponRepo(std::move(couponRepo)),
      taxRate(taxRate) {}

PriceBreakdown PricingService::calculatePrice(const std::vector<OrderItem>& items,
                                              double distanceKm,
                                              const std::string& couponCode) const {
    PriceBreakdown breakdown;
    double sum = 0.0;
    for (const auto& item : items) {
        sum += item.getTotalPrice();
    }
    breakdown.itemTotal = sum;

    // Delivery fee
    if (deliveryFeeStrategy) {
        breakdown.deliveryFee = deliveryFeeStrategy->calculateFee(distanceKm, sum);
    } else {
        breakdown.deliveryFee = 2.0; // default fallback
    }

    // Coupon discount
    if (!couponCode.empty() && couponRepo) {
        auto coupon = couponRepo->findByCode(couponCode);
        if (coupon && coupon->isValid(sum)) {
            breakdown.discount = coupon->calculateDiscount(sum);
            breakdown.couponCodeApplied = couponCode;
        }
    }

    // Tax calculation on discounted items
    double taxableAmount = std::max(0.0, breakdown.itemTotal - breakdown.discount);
    breakdown.tax = std::round((taxableAmount * taxRate) * 100.0) / 100.0;

    // Final total
    breakdown.finalAmount = taxableAmount + breakdown.tax + breakdown.deliveryFee;
    return breakdown;
}

} // namespace FoodSprint
