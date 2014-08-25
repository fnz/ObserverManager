#include "BaseObserverProtocol.h"
#include "ObserverManager.h"

BaseObserverProtocol::BaseObserverProtocol() {
    static unsigned int observersCount = 0;
    observerId = ++observersCount;
}

BaseObserverProtocol::~BaseObserverProtocol() {
    ObserverManager::unsubscribe(this);
}