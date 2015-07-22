#ifndef _IACODECOLLECTION_H_
#define _IACODECOLLECTION_H_

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <string>

/***
 * Collection of codeIA for Action
 * It use because we don't know the type of template
 */
extern std::unordered_map<std::type_index, std::string> IACodeCollection;

#endif //_IACODECOLLECTION_H_