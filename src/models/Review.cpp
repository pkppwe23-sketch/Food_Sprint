#include "models/Review.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

Review::Review(std::string reviewId, std::string orderId, std::string userId,
               std::string restaurantId, std::string deliveryPartnerId,
               double restaurantRating, double deliveryRating, std::string comment)
    : reviewId(std::move(reviewId)), orderId(std::move(orderId)),
      userId(std::move(userId)), restaurantId(std::move(restaurantId)),
      deliveryPartnerId(std::move(deliveryPartnerId)),
      restaurantRating(restaurantRating), deliveryRating(deliveryRating),
      comment(std::move(comment)) {}

std::string Review::toString() const {
    std::ostringstream oss;
    oss << "Review [" << reviewId << "] for Order " << orderId
        << " | Restaurant Rating: " << std::fixed << std::setprecision(1) << restaurantRating << "★"
        << " | Delivery Rating: " << deliveryRating << "★";
    if (!comment.empty()) {
        oss << " | Comment: \"" << comment << "\"";
    }
    return oss.str();
}

} // namespace FoodSprint
