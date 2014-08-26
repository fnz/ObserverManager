#pragma once

class BaseObserverProtocol {
public:
    BaseObserverProtocol();
    virtual ~BaseObserverProtocol();

    unsigned int observerId;
};
