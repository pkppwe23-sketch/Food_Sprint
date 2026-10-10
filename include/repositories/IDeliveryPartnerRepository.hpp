#pragma once

#include "models/DeliveryPartner.hpp"
#include <memory>
#include <vector>
#include <string>

namespace FoodSprint {

class IDeliveryPartnerRepository {
public:
    virtual ~IDeliveryPartnerRepository() = default;
    virtual void save(std::shared_ptr<DeliveryPartner> partner) = 0;
    virtual std::shared_ptr<DeliveryPartner> findById(const std::string& partnerId) const = 0;
    virtual std::vector<std::shared_ptr<DeliveryPartner>> findAll() const = 0;
    virtual std::vector<std::shared_ptr<DeliveryPartner>> findAvailablePartners() const = 0;
};

} // namespace FoodSprint
