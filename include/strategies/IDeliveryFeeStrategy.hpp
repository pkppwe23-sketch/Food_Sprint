#pragma once

#include <string>
#include <algorithm>

namespace FoodSprint {

class IDeliveryFeeStrategy {
public:
    virtual ~IDeliveryFeeStrategy() = default;
    virtual double calculateFee(double distanceKm, double orderTotal) const = 0;
    virtual std::string getStrategyName() const = 0;
};

class DistanceBasedDeliveryFeeStrategy : public IDeliveryFeeStrategy {
private:
    double baseFee;
    double perKmFee;
    double baseDistanceKm;

public:
    DistanceBasedDeliveryFeeStrategy(double baseFee = 2.0, double perKmFee = 0.8, double baseDistance = 3.0)
        : baseFee(baseFee), perKmFee(perKmFee), baseDistanceKm(baseDistance) {}

    double calculateFee(double distanceKm, double /*orderTotal*/) const override {
        if (distanceKm <= baseDistanceKm) {
            return baseFee;
        }
        return baseFee + ((distanceKm - baseDistanceKm) * perKmFee);
    }

    std::string getStrategyName() const override {
        return "Standard Distance-Based Fee";
    }
};

class SurgeDeliveryFeeStrategy : public IDeliveryFeeStrategy {
private:
    double baseFee;
    double perKmFee;
    double surgeMultiplier; // e.g. 1.5x during rain or rush hour
    std::string surgeReason;

public:
    SurgeDeliveryFeeStrategy(double surgeMultiplier = 1.5,
                             std::string reason = "Peak Rain / Rush Hour Surge",
                             double baseFee = 2.0, double perKmFee = 0.8)
        : baseFee(baseFee), perKmFee(perKmFee), surgeMultiplier(surgeMultiplier),
          surgeReason(std::move(reason)) {}

    double calculateFee(double distanceKm, double /*orderTotal*/) const override {
        double standard = (distanceKm <= 3.0) ? baseFee : (baseFee + (distanceKm - 3.0) * perKmFee);
        return standard * surgeMultiplier;
    }

    std::string getStrategyName() const override {
        return "Surge Pricing (" + surgeReason + " - " + std::to_string(surgeMultiplier).substr(0, 3) + "x)";
    }
};

class FreeDeliveryAboveThresholdStrategy : public IDeliveryFeeStrategy {
private:
    double threshold;
    DistanceBasedDeliveryFeeStrategy fallbackStrategy;

public:
    FreeDeliveryAboveThresholdStrategy(double minThreshold = 30.0)
        : threshold(minThreshold), fallbackStrategy() {}

    double calculateFee(double distanceKm, double orderTotal) const override {
        if (orderTotal >= threshold) {
            return 0.0;
        }
        return fallbackStrategy.calculateFee(distanceKm, orderTotal);
    }

    std::string getStrategyName() const override {
        return "Free Delivery on orders above $" + std::to_string((int)threshold);
    }
};

} // namespace FoodSprint
