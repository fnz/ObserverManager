#include <gtest/gtest.h>
#include <sstream>
#include <memory>

#include "ObserverManager.h"
#include "SampleProtocols.h"

// Some basic use cases
TEST(ObserverManager, Basic) {
    std::stringstream out;

    A* a1 = new A(out, "Tic");
    ObserverManager::subscribe<FooBarProtocol>(a1);
    
    A* a2 = new A(out, "Tic");
    ObserverManager::subscribe<FooBarProtocol>(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    // Check deletion
    delete a1;
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    // Check unsubscribing
    ObserverManager::unsubscribe(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");

    delete a2;

    EXPECT_EQ("Tic Tic Tac Tac Tic Tac ", out.str());
}

// Check one class implementing two protocols
TEST(ObserverManager, TwoProtocolsSubscribe) {
    std::stringstream out;

    B* b = new B(out);
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    delete b;

    EXPECT_EQ("Tic Tac Toe ", out.str());
}

// Check automatic unsubscribing with multiple protocols
TEST(ObserverManager, TwoProtocolsAutoUnsubscribe) {
    std::stringstream out;
    
    B* b = new B(out);
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    delete b;
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    EXPECT_EQ("", out.str());
}

// Check manual unsubscribing with multiple protocols
TEST(ObserverManager, TwoProtocolsManualUnsubscribe) {
    std::stringstream out;
    
    B* b = new B(out);
    ObserverManager::subscribe<TicTacProtocol>(b);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    //ObserverManager::unsubscribe(b2); - won't compile due to ambiguity

    ObserverManager::unsubscribe<TicTacProtocol>(b);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Toe");
    
    delete b;

    EXPECT_EQ("Tic Tac Toe Toe ", out.str());
}

// Check mixed unsubscribing with multiple protocols
TEST(ObserverManager, TwoProtocolsMixedUnsubscribe) {
    std::stringstream out;
    
    B* b = new B(out);
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
    
    EXPECT_EQ("Tic Tac Toe Toe ", out.str());
}

// Test clearing
TEST(ObserverManager, Clear)  {
    std::stringstream out;

    A* a1 = new A(out, "Tic");
    ObserverManager::subscribe<FooBarProtocol>(a1);
    
    A* a2 = new A(out, "Tic");
    ObserverManager::subscribe<FooBarProtocol>(a2);
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    ObserverManager::clear<FooBarProtocol>();
    
    ObserverManager::notify(&FooBarProtocol::foo);
    ObserverManager::notify(&FooBarProtocol::bar, "Tac");

    delete a1;
    delete a2;

    EXPECT_EQ("Tic Tic Tac Tac ", out.str());
}

// Test bad input
TEST(ObserverManager, BadInput) {
    std::stringstream out;
    
    A* aBad = nullptr;
    ObserverManager::subscribe<FooBarProtocol>(aBad);
    
    B* bBad = nullptr;
    ObserverManager::unsubscribe<FooBarProtocol>(bBad);
    
    ObserverManager::notify(&TicTacProtocol::ticTac);
    ObserverManager::notify(&FooBarProtocol::bar, "Bad");
    
    EXPECT_EQ("", out.str());
}

// Test shared ptrs usage
TEST(ObserverManager, SharedPtr)  {
    std::stringstream out;

    auto a = std::make_shared<A>(out, "Tic");
    ObserverManager::subscribe<FooBarProtocol>(a);

    auto b = std::make_shared<B>(out);
    ObserverManager::subscribe<FooBarProtocol>(b);
    
    ObserverManager::notify(&FooBarProtocol::foo);

    ObserverManager::unsubscribe<FooBarProtocol>(b);

    ObserverManager::notify(&FooBarProtocol::bar, "Tac");
    
    EXPECT_EQ("Tic Tac ", out.str());
}