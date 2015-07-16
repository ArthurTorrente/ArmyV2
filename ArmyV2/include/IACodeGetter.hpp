#ifndef _IACODEGETTER_H_
#define _IACODEGETTER_H_

#include <string>

class IACodeGetter
{
public:
    virtual std::string getCode() const = 0;
};

#endif //_IACODEGETTER_H_