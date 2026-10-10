#pragma once

#include "IDeliveryPartnerRepository.hpp"
#include <unordered_map>
#include <mutex>

namespace FoodSprint {

class InMemoryDeliveryPartnerRepository : public IDeliveryPartnerRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<DeliveryPartner>> partners;
    mutable std::mutex mtx;

public:
    void save(std::shared_ptr<DeliveryPartner> partner) override;
    std::shared_ptr<DeliveryPartner> findById(const std::string& partnerId) const override;
    std::vector<std::shared_ptr<DeliveryPartner>> findAll() const override;
    std::vector<std::shared_ptr<DeliveryPartner>> findAvailablePartners() const override;
};

} // namespace FoodSprint
