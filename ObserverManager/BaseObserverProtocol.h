#pragma once

#include <cstdint>

class BaseObserverProtocol {
    friend class ObserverManager;

public:
    BaseObserverProtocol();
    BaseObserverProtocol(const BaseObserverProtocol&);
    BaseObserverProtocol(BaseObserverProtocol&&);
    BaseObserverProtocol& operator=(const BaseObserverProtocol&);
    BaseObserverProtocol& operator=(BaseObserverProtocol&&);
    virtual ~BaseObserverProtocol();

private:
    void setNextObserverId();

private:
    using id_t = uint64_t;
    id_t observerId;
};
