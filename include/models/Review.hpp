#pragma once

#include <string>

namespace FoodSprint {

class Review {
private:
    std::string reviewId;
    std::string orderId;
    std::string userId;
    std::string restaurantId;
    std::string deliveryPartnerId;
    double restaurantRating;
    double deliveryRating;
    std::string comment;

public:
    Review(std::string reviewId, std::string orderId, std::string userId,
           std::string restaurantId, std::string deliveryPartnerId,
           double restaurantRating, double deliveryRating, std::string comment);

    const std::string& getReviewId() const { return reviewId; }
    const std::string& getOrderId() const { return orderId; }
    const std::string& getUserId() const { return userId; }
    const std::string& getRestaurantId() const { return restaurantId; }
    const std::string& getDeliveryPartnerId() const { return deliveryPartnerId; }
    double getRestaurantRating() const { return restaurantRating; }
    double getDeliveryRating() const { return deliveryRating; }
    const std::string& getComment() const { return comment; }

    std::string toString() const;
};

} // namespace FoodSprint
