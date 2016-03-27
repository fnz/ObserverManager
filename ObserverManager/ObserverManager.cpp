#include "ObserverManager.h"

std::unordered_map<BaseObserverProtocol::id_t, void (*)(BaseObserverProtocol*)> ObserverManager::unsubscribeMap;
