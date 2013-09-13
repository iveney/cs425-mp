#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <boost/lexical_cast.hpp>

template <typename T>
std::string to_string(T str) {
  return boost::lexical_cast<std::string>(str);
}

#endif /* end of include guard: _UTIL_H_ */

