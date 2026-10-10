#include "models/Payment.hpp"
#include <sstream>
#include <iomanip>

namespace FoodSprint {

Payment::Payment(std::string paymentId, std::string orderId, double amount,
                 PaymentMode mode, PaymentStatus status,
                 std::string transactionRef, std::string timestamp)
    : paymentId(std::move(paymentId)), orderId(std::move(orderId)),
      amount(amount), mode(mode), status(status),
      transactionReference(std::move(transactionRef)),
      timestamp(std::move(timestamp)) {}

std::string Payment::toString() const {
    std::ostringstream oss;
    oss << "Payment [" << paymentId << "] Order: " << orderId
        << " | Amount: $" << std::fixed << std::setprecision(2) << amount
        << " | Mode: " << paymentModeToString(mode)
        << " | Status: " << paymentStatusToString(status);
    if (!transactionReference.empty()) {
        oss << " | Ref: " << transactionReference;
    }
    return oss.str();
}

} // namespace FoodSprint
