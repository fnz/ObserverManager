#pragma once

#include <set>
#include <mutex>
#include "BaseObserverProtocol.h"

class BaseObserverProtocol;

template <typename T>
class ObserverContainer {
    friend class ObserverManager;

private:
    static void addObserver(T* obs) {
        std::unique_lock<std::mutex> lock(containerMutex);
        observers.insert(obs);
    }

    static void removeObserver(BaseObserverProtocol* observer) {
        std::unique_lock<std::mutex> lock(containerMutex);
        observers.erase(static_cast<T*>(observer));
    }

    static std::set<T*> getObservers() {
        std::unique_lock<std::mutex> lock(containerMutex);
        std::set<T*> observersCopy(observers);
        return observersCopy;
    }

    static void clear() {
        std::unique_lock<std::mutex> lock(containerMutex);
        observers.clear();
    }

    static std::set<T*> observers;
    static std::mutex containerMutex;
};

template <typename T>
std::set<T*> ObserverContainer<T>::observers;

template <typename T>
std::mutex ObserverContainer<T>::containerMutex;