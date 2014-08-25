#pragma once

class BaseObserverProtocol {
    friend class ObserverManager;

public:
    BaseObserverProtocol();
    virtual ~BaseObserverProtocol();

private:
    unsigned int observerId;
};
