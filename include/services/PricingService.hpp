#pragma once

#include "models/Order.hpp"
#include "strategies/IDeliveryFeeStrategy.hpp"
#include "repositories/ICouponRepository.hpp"
#include <memory>

namespace FoodSprint {

class PricingService {
private:
    std::shared_ptr<IDeliveryFeeStrategy> deliveryFeeStrategy;
    std::shared_ptr<ICouponRepository> couponRepo;
    double taxRate; // 0.05 for 5% tax

public:
    PricingService(std::shared_ptr<IDeliveryFeeStrategy> feeStrategy,
                   std::shared_ptr<ICouponRepository> couponRepo,
                   double taxRate = 0.05);

    void setDeliveryFeeStrategy(std::shared_ptr<IDeliveryFeeStrategy> strategy) {
        if (strategy) {
            deliveryFeeStrategy = strategy;
        }
    }

    std::shared_ptr<IDeliveryFeeStrategy> getDeliveryFeeStrategy() const {
        return deliveryFeeStrategy;
    }

    PriceBreakdown calculatePrice(const std::vector<OrderItem>& items,
                                  double distanceKm,
                                  const std::string& couponCode = "") const;
};

} // namespace FoodSprint
