#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <thread>

#include "ObserverManager.h"

#pragma mark - Aux

std::stringstream stream;

void clearStream() {
    stream.str(std::string());
    stream.clear();
}

bool checkResult(const std::string answer) {
    bool ret = (stream.str() == answer);
    clearStream();
    return ret;
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

bool runTest1() {
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

    delete a2;

    return checkResult("Tic Tic Tac Tac Tic Tac ");
}

bool runTest2() {
    // Check one class implementing two protocols
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    delete b;

    return checkResult("Tic Tac Toe ");
}

bool runTest3() {
    // Check automatic unsubscribing with multiple protocols
    
    B* b = new B();
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    delete b;
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    return checkResult("");
}

bool runTest4() {
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
    
    delete b;

    return checkResult("Tic Tac Toe Toe ");
}

bool runTest5() {
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
    
    return checkResult("Tic Tac Toe Toe ");
}

bool runTest6() {
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

    delete a1;
    delete a2;

    return checkResult("Tic Tic Tac Tac ");
}

bool runTest7() {
    // Clear test
    
    A* a = new A();
    ObserverManager::unsubscribe<FooBarProtocol>(a);
    
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");

    delete a;

    return checkResult("");
}

bool runTest8() {
    // Bad input test
    
    A* aBad = nullptr;
    ObserverManager::subscribe<FooBarProtocol>(aBad);
    
    B* bBad = nullptr;
    ObserverManager::unsubscribe<FooBarProtocol>(bBad);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Bad");
    
    return checkResult("");
}

bool runTest(int i) {
    switch (i) {
        case 1: return runTest1();
        case 2: return runTest2();
        case 3: return runTest3();
        case 4: return runTest4();
        case 5: return runTest5();
        case 6: return runTest6();
        case 7: return runTest7();
        case 8: return runTest8();
    }
    return false;
}

void runAllTests() {
    // Run all tests consequently

    for (int i = 1; i < 9; i++) {
        clearStream();
        bool isOK = runTest(i);
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
    bool isOK = true;
    do {
        for (int i = 1; i <= ids.size(); i++) {
            isOK &= runTest(i);
        }
    } while (std::next_permutation(ids.begin(), ids.end()));
    clock_t end = clock();

    std::cout << "Hardcore mode: ";
    std::cout << (isOK ? "OK" : "FAIL") << std::endl;
    
    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Hardcore mode time: " << elapsed << std::endl;
    std::cout << std::endl;
}

void runMapSpeedTest() {
    // Speed test for map vs unordered_map comparison
    int vectorSize = 10000000;
    
    clock_t begin = clock();
    std::vector<A*> aVector(vectorSize, new A());
    for (int i = 0; i < aVector.size(); i++) {
        ObserverManager::subscribe<FooBarProtocol>(aVector[i]);
    }

    for (int i = 0; i < aVector.size(); i++) {
        ObserverManager::unsubscribe(aVector[i]);
    }
    clock_t end = clock();

    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Map speed test time: " << elapsed << std::endl;
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

void runThreadTest() {
	// Simple test for multithreading support

    const int nThreads = 10;
    const int toWait = 5;

    auto threadFunction = [](){
        std::vector<A*> v;
        for (int i = 0; i < 100; i++) {
            A* a = new A();
            v.push_back(a);
        }

        for (auto a : v) {
            ObserverManager::subscribe<FooBarProtocol>(a);
        }
        for (auto a : v) {
            delete a;
        }
    };

    for (int i = 0; i < nThreads; i++) {
        std::thread t(threadFunction);
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(toWait));

    ObserverManager::notify(&FooBarProtocol::bar, "Ha");

    bool isOK = checkResult("");
    std::cout << "Thread test: ";
    std::cout << (isOK ? "OK" : "FAIL") << std::endl;

    ObserverManager::clear<FooBarProtocol>();
}

int main(int argc, const char * argv[]) {
    runAllTests();
    runAllTestsHardcoreMode();
    runThreadTest();
}
    


