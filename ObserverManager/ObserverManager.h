#pragma once

#include <unordered_map>
#include "ObserverContainer.h"
#include "BaseObserverProtocol.h"

class ObserverManager {
public:
    template <class T, class O>
    static void subscribe(O* observer) {
        if (!observer) {
            return;
        }
        unsubscribeMap[observer->T::observerId] = &ObserverContainer<T>::removeObserver;
        ObserverContainer<T>::addObserver(static_cast<T*>(observer));
    }

    template <class T>
    static void notify(void (T::*method)()) {
        auto currentObservers = ObserverContainer<T>::observers;
        for (auto observer : currentObservers) {
            (observer->*method)();
        }
    }

    // to disambiguate a call to overloaded member function
    // use notify(static_cast<void (Class::*)(T1, T2, ...)>(&Class::method), arg1, arg2, ...);
    template <typename T, typename... MethodArgumentTypes, typename... ActualArgumentTypes>
    static void notify(void (T::*method)(MethodArgumentTypes...), ActualArgumentTypes... args) {
        auto currentObservers = ObserverContainer<T>::observers;
        for (auto observer : currentObservers) {
            (observer->*method)(args...);
        }
    }

    template <class T>
    static void unsubscribe(T* observer) {
        if (!observer) {
            return;
        }
        if (unsubscribeMap.find(observer->T::observerId) == std::end(unsubscribeMap)) {
            return;
        }
        auto unsubscribeObserverFunc = unsubscribeMap[observer->T::observerId];
        if (unsubscribeObserverFunc != nullptr) {
            unsubscribeObserverFunc(observer);
        }
    }

    template <typename T>
    static void clear() {
        return ObserverContainer<T>::clear();
    }

private:
    static std::unordered_map<int, void (*)(BaseObserverProtocol*)> unsubscribeMap;
};