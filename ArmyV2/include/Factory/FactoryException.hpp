#ifndef _FACTORYEXCEPTION_H_
#define _FACTORYEXCEPTION_H_

#include <exception>

namespace Factory
{
    /**
    * Custom exception
    * It throw when factory has an error
    */
    class FactoryException : public std::exception
    {
    public:
        FactoryException()
            : std::exception()
        {}

        FactoryException(const char* m)
            : std::exception(m)
        {}

        FactoryException(const char* m, int i)
            : std::exception(m, i)
        {}

        FactoryException(const FactoryException& fe)
            : std::exception(fe)
        {}
    };
}
#endif //_FACTORYEXCEPTION_H_