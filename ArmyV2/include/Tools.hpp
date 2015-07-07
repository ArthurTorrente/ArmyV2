#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>

namespace tools
{
    template <typename T>
    void unusedArg(T value)
    {
        (void)value;
    }

    template < typename T, typename... TArgs >
    void unusedArg(T value, TArgs... args)
    {   
        (void)value;

        unusedArg(args...);
    }

    template<typename T>
    inline T clamp(T min, T max, T value)
    {
        return std::min(max, std::max(value, min));
    }
}

#endif //_TOOLS_H_