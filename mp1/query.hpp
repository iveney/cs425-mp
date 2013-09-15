#ifndef _QUERY_H_
#define _QUERY_H_

#include <string>
using std::string;

struct Query {
  enum QUERY_TYPE {KEY, VALUE, BOTH};

  Query(QUERY_TYPE type, const string& pattern, const string& filename)
    : type_(type), pattern_(pattern), filename_(filename) {}

  QUERY_TYPE type_;
  string pattern_;
  string filename_;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & type_;
    ar & pattern_;
    ar & filename_;
  }
};

#endif /* end of include guard: _QUERY_H_ */

