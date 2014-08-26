#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "ObserverManager.h"

#pragma mark - Aux

std::stringstream stream;
bool isOK;

void clearStream() {
    stream.str(std::string());
    stream.clear();
}

void checkResult(const std::string answer) {
    if (stream.str() != answer) {
        isOK = false;
    }
    clearStream();
}

#pragma mark - Sample protocols

class FooBarProtocol : public BaseObserverProtocol {
public:
    virtual void foo() {}
    virtual void bar(const std::string& word) {}
};

class TicTacProtocol : public BaseObserverProtocol {
public:
    virtual void ticTac() {}
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

class B : public FooBarProtocol, public TicTacProtocol {
public:
    virtual void ticTac() override {
        stream << "Tic Tac" << " ";
    }
    virtual void bar(const std::string& word) override {
        stream << word << " ";
    }
};

#pragma mark - Tests

void runTest1() {
    // Basic uses
    A* a1 = new A();
    a1->name = "Tic";
    ObserverManager::subscribe<FooBarProtocol>(a1);
    
    A* a2 = new A();
    a2->name = "Tic";
    ObserverManager::subscribe<FooBarProtocol>(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    // check deletion
    delete a1;
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    // check unsubscribing
    ObserverManager::unsubscribe(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    checkResult("Tic Tic Tac Tac Tic Tac ");

    delete a2;
}

void runTest2() {
    // Check one class implementing two protocols
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    checkResult("Tic Tac Toe ");
    
    delete b;
}

void runTest3() {
    // Check automatic unsubscribing with multiple protocols
    
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    delete b;
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    checkResult("");
}

void runTest4() {
    // Check manual unsubscribing with multiple protocols
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    //ObserverManager::unsubscribe(b2); won't compile due to ambiguity
    
    ObserverManager::unsubscribe<TicTacProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    checkResult("Tic Tac Toe Toe ");
    
    delete b;
}

void runTest5() {
    // Check mixed unsubscribing with multiple protocols
    
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    ObserverManager::unsubscribe<TicTacProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    delete b;
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    checkResult("Tic Tac Toe Toe ");
}

void runTest6() {
    // Clear test
    
    A* a1 = new A();
    a1->name = "Tic";
    ObserverManager::subscribe<FooBarProtocol>(a1);
    
    A* a2 = new A();
    a2->name = "Tic";
    ObserverManager::subscribe<FooBarProtocol>(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    ObserverManager::clear<FooBarProtocol>();
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    checkResult("Tic Tic Tac Tac ");

    delete a1;
    delete a2;
}

void runTest7() {
    // Clear test
    
    A* a = new A();
    ObserverManager::unsubscribe<FooBarProtocol>(a);
    
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    checkResult("");

    delete a;
}

void runTest8() {
    // Bad input test
    
    A* aBad = nullptr;
    ObserverManager::subscribe<FooBarProtocol>(aBad);
    
    B* bBad = nullptr;
    ObserverManager::unsubscribe<FooBarProtocol>(bBad);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Bad");
    
    checkResult("");
}

void runTest(int i) {
    switch (i) {
        case 1: runTest1(); break;
        case 2: runTest2(); break;
        case 3: runTest3(); break;
        case 4: runTest4(); break;
        case 5: runTest5(); break;
        case 6: runTest6(); break;
        case 7: runTest7(); break;
        case 8: runTest8(); break;
    }
}

void runAllTests() {
    // Run all tests consequently

    for (int i = 1; i < 9; i++) {
        isOK = true;
        clearStream();
        runTest(i);
        std::cout << "Test " << std::to_string(i) << ": ";
        std::cout << (isOK ? "OK" : "FAIL") << std::endl;
    }
    std::cout << std::endl;
}

void runAllTestsHardcoreMode() {
    // Run previous tests in all possible permutations
    // to ensure the results are order independent
    
    std::vector<int> ids;
    for (int i = 1; i < 9; i++) {
        ids.push_back(i);
    }
    
    clock_t begin = clock();
    do {
        for (int i = 1; i <= ids.size(); i++) {
            runTest(i);
        }
    } while (std::next_permutation(ids.begin(), ids.end()));
    clock_t end = clock();

    std::cout << "Hardcore mode: ";
    std::cout << (isOK ? "OK" : "FAIL") << std::endl;
    
    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Hardcore mode time: " << elapsed << std::endl;
    std::cout << std::endl;
}

void runSpeedTest() {
    // Speed test for different stl containers comparison
    int vectorSize = 1000;
    int loopSize = 1000000;
    
    std::vector<A*> aVector(vectorSize, new A());
    srand(0);
    
    clock_t begin = clock();
    
    for (int i = 0; i < loopSize; i++) {
        int choise = rand()%3;
        
        switch (choise) {
            case 0:
                ObserverManager::subscribe<FooBarProtocol>(aVector[rand()%vectorSize]);
                break;
                
            case 1:
                ObserverManager::notify(&FooBarProtocol::foo);
                break;
                
            case 2:
                ObserverManager::unsubscribe(aVector[rand()%vectorSize]);
                break;
        }
    }
    
    clock_t end = clock();
    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Speed test time: " << elapsed << std::endl;
    std::cout << std::endl;
}

int main(int argc, const char * argv[]) {
    runAllTests();
    runAllTestsHardcoreMode();
}
    


