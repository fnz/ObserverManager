#include <gtest/gtest.h>
#include <vector>
#include <sstream>

#include "ObserverManager.h"
#include "SampleProtocols.h"

TEST(ObserverManager, SpeedTest) {
    int vectorSize = 1000;
    int loopSize = 1000;
    
	std::stringstream out;

    std::vector<A*> aVector; 
	for (int i = 0; i < vectorSize; i++) {
		aVector.push_back(new A(out, ""));
	}

    for (auto a : aVector) {
    	ObserverManager::subscribe<FooBarProtocol>(a);
    }

    for (int i = 0; i < loopSize; i++) {
    	ObserverManager::notify(&FooBarProtocol::foo);
    }

    for (auto a : aVector) {
    	ObserverManager::unsubscribe(a);
    }

    for (auto a : aVector) {
    	delete a;
    }
    aVector.clear();
}