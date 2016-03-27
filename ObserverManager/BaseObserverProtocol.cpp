#include "BaseObserverProtocol.h"

#include "ObserverManager.h"

BaseObserverProtocol::BaseObserverProtocol() {
    setNextObserverId();
}

BaseObserverProtocol::BaseObserverProtocol(const BaseObserverProtocol&) {
    setNextObserverId();
}

BaseObserverProtocol::BaseObserverProtocol(BaseObserverProtocol&&) {
    setNextObserverId();
}

BaseObserverProtocol& BaseObserverProtocol::operator=(const BaseObserverProtocol&) {
    // retain observerId
    return *this;
}

BaseObserverProtocol& BaseObserverProtocol::operator=(BaseObserverProtocol&&) {
    // retain observerId
    return *this;
}

BaseObserverProtocol::~BaseObserverProtocol() {
    ObserverManager::unsubscribe(this);
}

void BaseObserverProtocol::setNextObserverId() {
    static id_t observersCount = 0;
    observerId = ++observersCount;
}