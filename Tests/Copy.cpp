#include "ObserverManager.h"
#include "TestProtocols.h"

#include <gtest/gtest.h>

// Some basic use cases
TEST(ObserverManager, Stack) {
    std::stringstream out;

    {
        A a(out, "A");
        ObserverManager::subscribe<FooBarProtocol>(&a);
        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A ", out.str());
    }

    ObserverManager::notify(&FooBarProtocol::foo);
    EXPECT_EQ("A ", out.str());
}

TEST(ObserverManager, Copy1) {
    std::stringstream out;

    {
        A a1(out, "A");
        ObserverManager::subscribe<FooBarProtocol>(&a1);

        A a2(a1);

        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A ", out.str());

        ObserverManager::subscribe<FooBarProtocol>(&a2);

        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A A A ", out.str());
    }

    ObserverManager::notify(&FooBarProtocol::foo);
    EXPECT_EQ("A A A ", out.str());
}

TEST(ObserverManager, Copy2) {
    std::stringstream out;

    {
        A a1(out, "A");
        ObserverManager::subscribe<FooBarProtocol>(&a1);

        A a2(out, "A");
        ObserverManager::subscribe<FooBarProtocol>(&a2);

        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A A ", out.str());

        a1.name = "B";
        a2 = a1;

        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A A B B ", out.str());

        a1.name = "C";
        ObserverManager::unsubscribe(&a1);

        ObserverManager::notify(&FooBarProtocol::foo);
        EXPECT_EQ("A A B B B ", out.str());
    }

    ObserverManager::notify(&FooBarProtocol::foo);
    EXPECT_EQ("A A B B B ", out.str());
}