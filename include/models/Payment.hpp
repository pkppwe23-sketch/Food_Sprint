#pragma once

#include <string>

namespace FoodSprint {

enum class PaymentMode {
    UPI,
    CREDIT_CARD,
    DEBIT_CARD,
    WALLET,
    CASH_ON_DELIVERY
};

enum class PaymentStatus {
    PENDING,
    SUCCESS,
    FAILED,
    REFUNDED
};

inline std::string paymentModeToString(PaymentMode mode) {
    switch (mode) {
        case PaymentMode::UPI:              return "UPI";
        case PaymentMode::CREDIT_CARD:      return "Credit Card";
        case PaymentMode::DEBIT_CARD:       return "Debit Card";
        case PaymentMode::WALLET:           return "Wallet";
        case PaymentMode::CASH_ON_DELIVERY: return "Cash On Delivery";
        default:                            return "Unknown";
    }
}

inline std::string paymentStatusToString(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::PENDING:  return "PENDING";
        case PaymentStatus::SUCCESS:  return "SUCCESS";
        case PaymentStatus::FAILED:   return "FAILED";
        case PaymentStatus::REFUNDED: return "REFUNDED";
        default:                      return "UNKNOWN";
    }
}

class Payment {
private:
    std::string paymentId;
    std::string orderId;
    double amount;
    PaymentMode mode;
    PaymentStatus status;
    std::string transactionReference;
    std::string timestamp;

public:
    Payment(std::string paymentId, std::string orderId, double amount,
            PaymentMode mode, PaymentStatus status = PaymentStatus::PENDING,
            std::string transactionRef = "", std::string timestamp = "");

    const std::string& getPaymentId() const { return paymentId; }
    const std::string& getOrderId() const { return orderId; }
    double getAmount() const { return amount; }
    PaymentMode getMode() const { return mode; }
    PaymentStatus getStatus() const { return status; }
    const std::string& getTransactionReference() const { return transactionReference; }
    const std::string& getTimestamp() const { return timestamp; }

    void setStatus(PaymentStatus newStatus) { status = newStatus; }
    void setTransactionReference(const std::string& ref) { transactionReference = ref; }
    void setTimestamp(const std::string& ts) { timestamp = ts; }

    std::string toString() const;
};

} // namespace FoodSprint
