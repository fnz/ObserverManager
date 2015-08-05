ObserverManager
===============

Generalized observer pattern based on C++11 variadic templates.

This implementation of the Observer pattern allows you to group events in interfaces (further called protocols) with the ability to parametrize them as you like. A single observer can be subscribed to different groups of events simultaneously. All observers are automatically unsubscribed when their destructor is called, so no memory issues will arise. Observers are passed to manager as pointers, shared pointers are supported as well.

Here's a sample use:

```
#include <iostream>

#include "ObserverManager.h"

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

int main() {
	Worker* jim = new Worker();
	ObserverManager::subscribe<MoneyProtocol>(jim);

	ObserverManager::notify(&MoneyProtocol::add, 500);
	ObserverManager::notify(&MoneyProtocol::withdraw, 100, "SuperBurger");

	ObserverManager::unsubscribe(jim);

	delete jim;
}
```

To clone and run tests:

```
git clone --recursive https://github.com/fnz/ObserverManager.git
cd ObserverManager
./run_tests.sh
```
