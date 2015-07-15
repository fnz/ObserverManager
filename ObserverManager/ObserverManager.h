#pragma once

#include <map>
#include <vector>
#include <memory>

#include "ObserverContainer.h"
#include "BaseObserverProtocol.h"

class ObserverManager {
public:
    // P stands for current protocol (a group of parametrized events)
    // O is a class, overriding some of the protocol's methods, this defining it's reaction to events 
    template <class P, class O>
    static void subscribe(O* observer) {
        if (!observer) {
            return;
        }
        unsubscribeMap[observer->P::observerId] = &ObserverContainer<P>::removeObserver;
        ObserverContainer<P>::addObserver(static_cast<P*>(observer));
    }

    template <class P, class O>
    static void subscribe(const std::shared_ptr<O>& observer) {
        subscribe<P>(observer.get());
    }

    // to disambiguate a call to overloaded member function
    // use notify(static_cast<void (Class::*)(T1, T2, ...)>(&Class::method), arg1, arg2, ...);
    template <class P, typename... MethodArgumentTypes, typename... ActualArgumentTypes>
    static void notify(void (P::*method)(MethodArgumentTypes...), ActualArgumentTypes... args) {
        std::vector<P*> currentObservers;
        std::copy(std::begin(ObserverContainer<P>::observers), std::end(ObserverContainer<P>::observers), std::back_inserter(currentObservers));
        std::vector<unsigned int> currentObserverIds;
        for (unsigned int i = 0; i < currentObservers.size(); i++) {
            currentObserverIds.push_back(currentObservers[i]->observerId);
        }
        for (unsigned int i = 0; i < currentObservers.size(); i++) {
            if (unsubscribeMap.find(currentObserverIds[i]) == std::end(unsubscribeMap)) {
                continue;
            }
            (currentObservers[i]->*method)(args...);
        }
    }

    template <class P>
    static void unsubscribe(P* observer) {
        if (!observer) {
            return;
        }
        auto observerId = observer->P::observerId;
        if (unsubscribeMap.find(observerId) == std::end(unsubscribeMap)) {
            return;
        }
        auto unsubscribeObserverFunc = unsubscribeMap[observerId];
        if (unsubscribeObserverFunc != nullptr) {
            unsubscribeObserverFunc(observer);
        }
        unsubscribeMap.erase(observerId);
    }

    template <class P>
    static void unsubscribe(const std::shared_ptr<P>& observer) {
        unsubscribe<P>(observer.get());
    }

    template <class P>
    static void clear() {
        ObserverContainer<P>::clear();
        unsubscribeMap.clear();
    }

private:
    static std::map<int, void (*)(BaseObserverProtocol*)> unsubscribeMap;
};