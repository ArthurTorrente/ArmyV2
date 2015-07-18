#ifndef _TOOLS_H_
#define _TOOLS_H_

#pragma warning(push, 0)
#include <algorithm>
#pragma warning(pop)

namespace tools
{
    /**
     * templated function uses for, unused parameter function
     */
    template <typename T>
    void unusedArg(const T& value)
    {
        (void)value;
    }

    /**
    * Variadic template uses for remove the warnings, unused parameter function
    */
    template < typename T, typename... TArgs >
    void unusedArg(const T& value, TArgs... args)
    {   
        (void)value;

        unusedArg(args...);
    }

    /**
     * Clamp a value between range
     */
    template<typename T>
    inline T clamp(T min, T max, T value)
    {
        return std::min(max, std::max(value, min));
    }
}

#endif //_TOOLS_H_