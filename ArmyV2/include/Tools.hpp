#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>

namespace tools
{
    template<typename T>
    inline T clamp(T min, T max, T value)
    {
        return std::min(max, std::max(value, min));
    }
}

#endif //_TOOLS_H_