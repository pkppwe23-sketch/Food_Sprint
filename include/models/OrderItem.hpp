#pragma once

#include "MenuItem.hpp"
#include <memory>
#include <string>

namespace FoodSprint {

class OrderItem {
private:
    std::shared_ptr<MenuItem> menuItem;
    int quantity;
    double unitPrice;
    std::string specialInstructions;

public:
    OrderItem(std::shared_ptr<MenuItem> item, int quantity,
              std::string instructions = "");

    std::shared_ptr<MenuItem> getMenuItem() const { return menuItem; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    const std::string& getSpecialInstructions() const { return specialInstructions; }

    void setQuantity(int qty) { quantity = qty; }
    double getTotalPrice() const { return unitPrice * quantity; }

    std::string toString() const;
};

} // namespace FoodSprint
