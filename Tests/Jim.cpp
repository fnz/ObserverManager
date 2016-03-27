#include "ObserverManager.h"

#include <gtest/gtest.h>
#include <iostream>

class MoneyProtocol : public BaseObserverProtocol {
public:
    virtual void add(unsigned int sum) {}
    virtual void withdraw(unsigned int sum, const std::string& item) {}
};

class Worker : public MoneyProtocol {
public:
    virtual void add(const unsigned int sum) override {
        std::cout << "Look, dear, I've earned " << std::to_string(sum) << "$ today!" << std::endl;
    }

    virtual void withdraw(unsigned int sum, const std::string& item) override {
        std::cout << "Oh, I've spent " <<  std::to_string(sum) << "$ on " << item << " today..." << std::endl;
    }
};

TEST(ObserverManager, Jim) {
    Worker* jim = new Worker();
    ObserverManager::subscribe<MoneyProtocol>(jim);

    ObserverManager::notify(&MoneyProtocol::add, 500);
    ObserverManager::notify(&MoneyProtocol::withdraw, 100, "SuperBurger");

    ObserverManager::unsubscribe(jim);

    delete jim;
}