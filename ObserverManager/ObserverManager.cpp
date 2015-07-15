#include "ObserverManager.h"

std::map<int, void (*)(BaseObserverProtocol*)> ObserverManager::unsubscribeMap;
