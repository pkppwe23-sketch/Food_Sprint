#include "models/OrderItem.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

OrderItem::OrderItem(std::shared_ptr<MenuItem> item, int quantity,
                     std::string instructions)
    : menuItem(item), quantity(quantity),
      unitPrice(item ? item->getPrice() : 0.0),
      specialInstructions(std::move(instructions)) {}

std::string OrderItem::toString() const {
    std::ostringstream oss;
    if (menuItem) {
        oss << menuItem->getName() << " x " << quantity << " @ $"
            << std::fixed << std::setprecision(2) << unitPrice << " = $"
            << getTotalPrice();
        if (!specialInstructions.empty()) {
            oss << " (Note: " << specialInstructions << ")";
        }
    } else {
        oss << "Invalid Item";
    }
    return oss.str();
}

} // namespace FoodSprint
