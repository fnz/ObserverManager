#include "ObserverManager.h"
#include "TestProtocols.h"

#include <gtest/gtest.h>
#include <vector>
#include <sstream>

TEST(ObserverManager, SpeedTest) {
    const int vectorSize = 1000;
    const int loopSize = 1000;
    
    std::stringstream out;

    std::vector<A*> v;
    for (int i = 0; i < vectorSize; i++) {
        v.push_back(new A(out, ""));
    }

    for (int i = 0; i < loopSize; i++) {
        for (auto a : v) {
            ObserverManager::subscribe<FooBarProtocol>(a);
        }

        ObserverManager::notify(&FooBarProtocol::foo);

        for (auto a : v) {
            ObserverManager::unsubscribe(a);
        }
    }

    for (auto a : v) {
        delete a;
    }
}