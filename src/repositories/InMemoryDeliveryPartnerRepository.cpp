#include "repositories/InMemoryDeliveryPartnerRepository.hpp"

namespace FoodSprint {

void InMemoryDeliveryPartnerRepository::save(std::shared_ptr<DeliveryPartner> partner) {
    if (!partner) return;
    std::lock_guard<std::mutex> lock(mtx);
    partners[partner->getId()] = partner;
}

std::shared_ptr<DeliveryPartner> InMemoryDeliveryPartnerRepository::findById(const std::string& partnerId) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = partners.find(partnerId);
    if (it != partners.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<DeliveryPartner>> InMemoryDeliveryPartnerRepository::findAll() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<DeliveryPartner>> result;
    for (const auto& pair : partners) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<std::shared_ptr<DeliveryPartner>> InMemoryDeliveryPartnerRepository::findAvailablePartners() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::shared_ptr<DeliveryPartner>> result;
    for (const auto& pair : partners) {
        if (pair.second && pair.second->getIsAvailable()) {
            result.push_back(pair.second);
        }
    }
    return result;
}

} // namespace FoodSprint
