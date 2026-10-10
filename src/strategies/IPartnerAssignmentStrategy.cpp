#include "strategies/IPartnerAssignmentStrategy.hpp"
#include <limits>

namespace FoodSprint {

std::shared_ptr<DeliveryPartner> NearestPartnerAssignmentStrategy::assignPartner(
    const std::vector<std::shared_ptr<DeliveryPartner>>& availablePartners,
    const Location& restaurantLocation) {

    std::shared_ptr<DeliveryPartner> bestPartner = nullptr;
    double minDistance = std::numeric_limits<double>::max();

    for (const auto& partner : availablePartners) {
        if (!partner || !partner->getIsAvailable()) continue;
        double dist = partner->getCurrentLocation().distanceTo(restaurantLocation);
        if (dist < minDistance) {
            minDistance = dist;
            bestPartner = partner;
        }
    }
    return bestPartner;
}

std::shared_ptr<DeliveryPartner> HighestRatedPartnerAssignmentStrategy::assignPartner(
    const std::vector<std::shared_ptr<DeliveryPartner>>& availablePartners,
    const Location& restaurantLocation) {

    std::shared_ptr<DeliveryPartner> bestPartner = nullptr;
    double highestRating = -1.0;

    for (const auto& partner : availablePartners) {
        if (!partner || !partner->getIsAvailable()) continue;
        double dist = partner->getCurrentLocation().distanceTo(restaurantLocation);
        if (dist <= maxRadiusKm) {
            if (partner->getRating() > highestRating) {
                highestRating = partner->getRating();
                bestPartner = partner;
            }
        }
    }
    // Fallback if none in radius: pick nearest
    if (!bestPartner) {
        NearestPartnerAssignmentStrategy nearest;
        return nearest.assignPartner(availablePartners, restaurantLocation);
    }
    return bestPartner;
}

} // namespace FoodSprint
