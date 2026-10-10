#pragma once

#include "models/Payment.hpp"
#include <memory>
#include <string>

namespace FoodSprint {

class IPaymentStrategy {
public:
    virtual ~IPaymentStrategy() = default;
    virtual std::shared_ptr<Payment> processPayment(const std::string& orderId, double amount) = 0;
    virtual bool refundPayment(std::shared_ptr<Payment> payment) = 0;
    virtual PaymentMode getPaymentMode() const = 0;
};

class UpiPaymentStrategy : public IPaymentStrategy {
private:
    std::string vpa;

public:
    explicit UpiPaymentStrategy(std::string upiId) : vpa(std::move(upiId)) {}

    std::shared_ptr<Payment> processPayment(const std::string& orderId, double amount) override;
    bool refundPayment(std::shared_ptr<Payment> payment) override;
    PaymentMode getPaymentMode() const override { return PaymentMode::UPI; }
};

class CreditCardPaymentStrategy : public IPaymentStrategy {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string cvv;
    std::string expiry;

public:
    CreditCardPaymentStrategy(std::string cardNum, std::string name, std::string cvv, std::string exp)
        : cardNumber(std::move(cardNum)), cardHolder(std::move(name)), cvv(std::move(cvv)), expiry(std::move(exp)) {}

    std::shared_ptr<Payment> processPayment(const std::string& orderId, double amount) override;
    bool refundPayment(std::shared_ptr<Payment> payment) override;
    PaymentMode getPaymentMode() const override { return PaymentMode::CREDIT_CARD; }
};

class WalletPaymentStrategy : public IPaymentStrategy {
private:
    std::string walletId;
    double currentBalance;

public:
    WalletPaymentStrategy(std::string walletId, double initialBalance)
        : walletId(std::move(walletId)), currentBalance(initialBalance) {}

    double getBalance() const { return currentBalance; }
    void addBalance(double amount) { currentBalance += amount; }

    std::shared_ptr<Payment> processPayment(const std::string& orderId, double amount) override;
    bool refundPayment(std::shared_ptr<Payment> payment) override;
    PaymentMode getPaymentMode() const override { return PaymentMode::WALLET; }
};

class CashOnDeliveryStrategy : public IPaymentStrategy {
public:
    std::shared_ptr<Payment> processPayment(const std::string& orderId, double amount) override;
    bool refundPayment(std::shared_ptr<Payment> payment) override;
    PaymentMode getPaymentMode() const override { return PaymentMode::CASH_ON_DELIVERY; }
};

} // namespace FoodSprint
