#include "query.h"

std::ostream& operator << (std::ostream& os, Query& query) {
  os << "Type: " << query.type_string();
  os << " Pattern: " << query.pattern_;
  os << " Filename: " << query.filename_;
  os << "\n";
  return os;
}

const string Query::type_string() const {
  string str;
  switch (type_) {
    case KEY:
      str = "key";
      break;
    case VALUE:
      str = "value";
      break;
    case BOTH:
      str = "both";
      break;
  }
  return str;
}
