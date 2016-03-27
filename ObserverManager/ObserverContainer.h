#pragma once

#include "BaseObserverProtocol.h"

#include <unordered_set>

template <typename T>
class ObserverContainer {
    friend class ObserverManager;

private:
    static std::unordered_set<T*> observers;

    static void addObserver(T* obs) {
        observers.insert(obs);
    }

    static void removeObserver(BaseObserverProtocol* observer) {
        observers.erase(static_cast<T*>(observer));
    }

    static void clear() {
        observers.clear();
    }
};

template <typename T>
std::unordered_set<T*> ObserverContainer<T>::observers;