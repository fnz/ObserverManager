#pragma once

#include "ObserverContainer.h"
#include "BaseObserverProtocol.h"

#include <memory>
#include <unordered_map>
#include <vector>

class ObserverManager {
public:
    // P stands for current protocol (a group of parametrized events)
    // O is a class overriding some of the protocol's methods, defining it's reaction to events
    // Two template parameters are needed to explicitly state what protocol current observer
    // is being subscribed to, as observer can subscribe to multiple protocols
    template <class P, class O>
    static void subscribe(O* observer) {
        static_assert(std::is_base_of<BaseObserverProtocol, O>::value,
                      "All observers must inherit from BaseObserverProtocol");
        if (observer == nullptr) {
            return;
        }

        unsubscribeMap[observer->P::observerId] = &ObserverContainer<P>::removeObserver;
        ObserverContainer<P>::addObserver(observer);
    }

    // Subscribing does not increase reference count
    template <class P, class O>
    static void subscribe(const std::shared_ptr<O>& observer) {
        subscribe<P>(observer.get());
    }

    // to disambiguate a call to overloaded member function
    // use notify(static_cast<void (Class::*)(T1, T2, ...)>(&Class::method), arg1, arg2, ...);
    template <class P, typename... MethodArgumentTypes, typename... ActualArgumentTypes>
    static void notify(void (P::*method)(MethodArgumentTypes...), ActualArgumentTypes... args) {
        std::vector<std::pair<P*, BaseObserverProtocol::id_t>> observers;
        for (const auto& o : ObserverContainer<P>::observers) {
            observers.push_back(std::make_pair(o, o->observerId));
        }

        for (const auto& p : observers) {
            if (unsubscribeMap.find(p.second) == std::end(unsubscribeMap)) {
                continue;
            }
            (p.first->*method)(args...);
        }
    }

    // Only one type is used here to make destructors able to automatically unsubscribe observers
    // If user attempts to manually unsubscribe an observer with two protocols, he will get a compiler error
    template <class P>
    static void unsubscribe(P* observer) {
        if (observer == nullptr) {
            return;
        }

        auto id = observer->P::observerId;
        auto it = unsubscribeMap.find(id);
        if (it == std::end(unsubscribeMap)) {
            return;
        }

        auto ptr = it->second;
        if (ptr != nullptr) {
            ptr(observer);
        }

        unsubscribeMap.erase(id);
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
    static std::unordered_map<BaseObserverProtocol::id_t, void (*)(BaseObserverProtocol*)> unsubscribeMap;
};