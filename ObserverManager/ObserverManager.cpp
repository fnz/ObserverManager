#include "ObserverManager.h"

std::unordered_map<int, void (*)(BaseObserverProtocol*)> ObserverManager::unsubscribeMap;
