#pragma once

#include <string>

namespace FoodSprint {

enum class DietaryPreference {
    VEG,
    NON_VEG,
    VEGAN
};

inline std::string dietaryPreferenceToString(DietaryPreference pref) {
    switch (pref) {
        case DietaryPreference::VEG:     return "Veg";
        case DietaryPreference::NON_VEG: return "Non-Veg";
        case DietaryPreference::VEGAN:   return "Vegan";
        default:                         return "Unknown";
    }
}

} // namespace FoodSprint
