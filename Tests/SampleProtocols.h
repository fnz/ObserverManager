#include <ostream>

#include "ObserverManager.h"

class FooBarProtocol : public BaseObserverProtocol {
public:
    virtual void foo() {}
    virtual void bar(const std::string& word) {}
};

class TicTacProtocol : public BaseObserverProtocol {
public:
    virtual void ticTac() {}
};

class StreamWriter {
public: 
    StreamWriter(std::ostream& outStream) :
    stream(outStream) {}

protected:
    std::ostream& stream;
};

class A : StreamWriter, public FooBarProtocol {
public:
    A(std::ostream& outStream, const std::string& objName) :
    StreamWriter(outStream), 
    name(objName) {}
    
    virtual void foo() override {
        stream << name << " ";
    };
    virtual void bar(const std::string& word) override {
        stream << word << " ";
    }

private:
    std::string name;
};

class B : StreamWriter, public FooBarProtocol, public TicTacProtocol {
public:
    B(std::ostream& outStream) :
    StreamWriter(outStream) {}

    virtual void ticTac() override {
        stream << "Tic Tac" << " ";
    }
    virtual void bar(const std::string& word) override {
        stream << word << " ";
    }
};