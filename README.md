ObserverManager
===============

Generalized observer pattern based on C++11 variadic templates. An observer can be subscribed to different groups of events. All observers are automatically unsubscribed when destructor is called. Parametrized events are supported with unlimited number of arguments.

Here's a sample use:

```
class FooBarProtocol : public BaseObserverProtocol {
public:
	virtual void foo() {}
	virtual void bar(const std::string& word) {}
};

class A : public FooBarProtocol {
public:
	std::string name;
	
	virtual void foo() override {
		stream << name << " ";
	};
	virtual void bar(const std::string& word) override {
		stream << word << " ";
	}
};

void main() {
	A* a = new A();
	a->name = "Mary";
	ObserverManager::subscribe<FooBarProtocol>(a);

	ObserverManager::notify(&FooBarProtocol::foo);
	ObserverManager::notify(&FooBarProtocol::bar, "Jane");
}
```

To run tests simply type 

```
make
./RunTests
```
