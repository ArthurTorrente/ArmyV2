#ifndef _IFILTERS_H_h
#define _IFILTERS_H_h

#include <memory>
#include <vector>

#include "Army.hpp"

template<typename OUT>
class IFilter
{
public:
    IFilter();
    virtual ~IFilter();

    virtual OUT operator()(std::shared_ptr<Unit>, const std::unique_ptr<Army>&, const std::unique_ptr<Army>&) = 0;
};

template<typename OUT, typename IN>
class FilterWithInput : public Filter < OUT >
{
    virtual OUT operator(std::shared_ptr<Unit>, const std::unique_ptr<Army>&, const std::unique_ptr<Army>&, IN);
};


#endif //_IFILTERS_H_h