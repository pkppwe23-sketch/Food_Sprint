#include "strategies/IPaymentStrategy.hpp"
#include <iostream>
#include <chrono>
#include <sstream>

namespace FoodSprint {

static std::string generateTransactionId(const std::string& prefix) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    return prefix + "_" + std::to_string(ms);
}

// UPI
std::shared_ptr<Payment> UpiPaymentStrategy::processPayment(const std::string& orderId, double amount) {
    std::string txId = generateTransactionId("UPI_TXN");
    std::cout << "  [Payment Gateway] Processing UPI payment of $" << amount
              << " via VPA: " << vpa << " ... Verified!\n";
    return std::make_shared<Payment>(txId, orderId, amount, PaymentMode::UPI,
                                     PaymentStatus::SUCCESS, "UPI-REF-" + vpa, "2026-10-10 12:00:00");
}

bool UpiPaymentStrategy::refundPayment(std::shared_ptr<Payment> payment) {
    if (payment && payment->getStatus() == PaymentStatus::SUCCESS) {
        payment->setStatus(PaymentStatus::REFUNDED);
        std::cout << "  [Payment Gateway] Refund of $" << payment->getAmount()
                  << " initiated to UPI VPA: " << vpa << ".\n";
        return true;
    }
    return false;
}

// Credit Card
std::shared_ptr<Payment> CreditCardPaymentStrategy::processPayment(const std::string& orderId, double amount) {
    std::string txId = generateTransactionId("CC_TXN");
    std::string masked = "XXXX-XXXX-XXXX-" + (cardNumber.length() >= 4 ? cardNumber.substr(cardNumber.length() - 4) : "0000");
    std::cout << "  [Payment Gateway] Authorizing Card ending in " << masked
              << " for $" << amount << " ... Approved!\n";
    return std::make_shared<Payment>(txId, orderId, amount, PaymentMode::CREDIT_CARD,
                                     PaymentStatus::SUCCESS, "CC-AUTH-9988", "2026-10-10 12:00:00");
}

bool CreditCardPaymentStrategy::refundPayment(std::shared_ptr<Payment> payment) {
    if (payment && payment->getStatus() == PaymentStatus::SUCCESS) {
        payment->setStatus(PaymentStatus::REFUNDED);
        std::cout << "  [Payment Gateway] Refund of $" << payment->getAmount()
                  << " reversed to Credit Card ending in "
                  << (cardNumber.length() >= 4 ? cardNumber.substr(cardNumber.length() - 4) : "0000") << ".\n";
        return true;
    }
    return false;
}

// Wallet
std::shared_ptr<Payment> WalletPaymentStrategy::processPayment(const std::string& orderId, double amount) {
    std::string txId = generateTransactionId("WLT_TXN");
    if (currentBalance >= amount) {
        currentBalance -= amount;
        std::cout << "  [Payment Gateway] Debited $" << amount
                  << " from Wallet [" << walletId << "]. Remaining balance: $" << currentBalance << "\n";
        return std::make_shared<Payment>(txId, orderId, amount, PaymentMode::WALLET,
                                         PaymentStatus::SUCCESS, "WLT-DEBIT-OK", "2026-10-10 12:00:00");
    } else {
        std::cout << "  [Payment Gateway] Wallet balance ($" << currentBalance
                  << ") insufficient for amount $" << amount << "!\n";
        return std::make_shared<Payment>(txId, orderId, amount, PaymentMode::WALLET,
                                         PaymentStatus::FAILED, "ERR_INSUFFICIENT_FUNDS", "2026-10-10 12:00:00");
    }
}

bool WalletPaymentStrategy::refundPayment(std::shared_ptr<Payment> payment) {
    if (payment && payment->getStatus() == PaymentStatus::SUCCESS) {
        currentBalance += payment->getAmount();
        payment->setStatus(PaymentStatus::REFUNDED);
        std::cout << "  [Payment Gateway] Credited $" << payment->getAmount()
                  << " back to Wallet [" << walletId << "]. Current balance: $" << currentBalance << "\n";
        return true;
    }
    return false;
}

// Cash on Delivery
std::shared_ptr<Payment> CashOnDeliveryStrategy::processPayment(const std::string& orderId, double amount) {
    std::string txId = generateTransactionId("COD_TXN");
    std::cout << "  [Payment Gateway] Cash On Delivery selected. Payable upon delivery: $" << amount << "\n";
    return std::make_shared<Payment>(txId, orderId, amount, PaymentMode::CASH_ON_DELIVERY,
                                     PaymentStatus::PENDING, "COD_PENDING", "2026-10-10 12:00:00");
}

bool CashOnDeliveryStrategy::refundPayment(std::shared_ptr<Payment> payment) {
    if (payment) {
        payment->setStatus(PaymentStatus::REFUNDED);
        std::cout << "  [Payment Gateway] COD Order cancelled. No cash collected.\n";
        return true;
    }
    return false;
}

} // namespace FoodSprint
