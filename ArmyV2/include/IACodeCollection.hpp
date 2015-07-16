#ifndef _IACODECOLLECTION_H_
#define _IACODECOLLECTION_H_

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <string>

extern std::unordered_map<std::type_index, std::string> IACodeCollection;

#endif //_IACODECOLLECTION_H_