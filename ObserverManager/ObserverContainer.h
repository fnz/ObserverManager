#pragma once

#include <set>
#include "BaseObserverProtocol.h"

class BaseObserverProtocol;

template <typename T>
class ObserverContainer {
    friend class ObserverManager;

private:
    static std::set<T*> observers;

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
std::set<T*> ObserverContainer<T>::observers;