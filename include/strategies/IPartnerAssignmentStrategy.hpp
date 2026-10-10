#pragma once

#include "models/DeliveryPartner.hpp"
#include "models/Location.hpp"
#include <vector>
#include <memory>
#include <string>

namespace FoodSprint {

class IPartnerAssignmentStrategy {
public:
    virtual ~IPartnerAssignmentStrategy() = default;
    virtual std::shared_ptr<DeliveryPartner> assignPartner(
        const std::vector<std::shared_ptr<DeliveryPartner>>& availablePartners,
        const Location& restaurantLocation) = 0;
    virtual std::string getStrategyName() const = 0;
};

class NearestPartnerAssignmentStrategy : public IPartnerAssignmentStrategy {
public:
    std::shared_ptr<DeliveryPartner> assignPartner(
        const std::vector<std::shared_ptr<DeliveryPartner>>& availablePartners,
        const Location& restaurantLocation) override;

    std::string getStrategyName() const override {
        return "Nearest Available Partner (Proximity-based)";
    }
};

class HighestRatedPartnerAssignmentStrategy : public IPartnerAssignmentStrategy {
private:
    double maxRadiusKm;

public:
    explicit HighestRatedPartnerAssignmentStrategy(double maxRadius = 10.0)
        : maxRadiusKm(maxRadius) {}

    std::shared_ptr<DeliveryPartner> assignPartner(
        const std::vector<std::shared_ptr<DeliveryPartner>>& availablePartners,
        const Location& restaurantLocation) override;

    std::string getStrategyName() const override {
        return "Highest-Rated Partner within " + std::to_string((int)maxRadiusKm) + "km";
    }
};

} // namespace FoodSprint
