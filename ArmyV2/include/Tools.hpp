#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>

namespace tools
{
    template<typename T>
    inline T clamp(T min, T max, t value)
    {
        return std::min(max, std::min(value, max));
    }
}

#endif //_TOOLS_H_