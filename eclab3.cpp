#include <iostream>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << balance << std::endl;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited $" << amount << " into the account." << std::endl;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the account." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance." << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        account.displayDetails();
        return os;
    }

    friend Account operator+(Account& lhs, Account& rhs);
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100;
        if (amount > 0 && (balance - amount) >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the savings account." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance to maintain the minimum balance." << std::endl;
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (amount > 0 && (balance - amount) >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from the current account." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or exceeding overdraft limit." << std::endl;
        }
    }

    // Custom assignment operator for CurrentAccount
    CurrentAccount& operator=(const Account& other) {
        if (this != &other) {
            const CurrentAccount* derived = dynamic_cast<const CurrentAccount*>(&other);
            if (derived) {
                // Copy derived properties
                overdraftLimit = derived->overdraftLimit;
            }
            // Copy base class properties
            Account::operator=(other);
        }
        return *this;
    }
};

// Implement the operator+ outside the class definition
Account operator+(Account& lhs, Account& rhs) {
    double transferAmount = std::min(lhs.balance, rhs.balance);
    lhs.balance -= transferAmount;
    rhs.balance += transferAmount;
    std::cout << "Transferred $" << transferAmount << " from one account to another." << std::endl;
    return rhs;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    // Transfer $300 from savings to current
    current = current + savings;

    std::cout << savings;
    std::cout << current;

    return 0;
}
