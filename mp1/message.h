#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Modified from chat room example from boost.asio

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string;

class Message {
public:
  enum { header_length = 4 };
  enum { max_body_length = 512 };

  Message()
    : body_length_(0)
  {
  }

  Message(const string& str) : body_length_(str.length()) {
    memcpy(body(), str.c_str(), body_length());
    encode_header();
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  size_t body_length() const
  {
    return body_length_;
  }

  void body_length(size_t new_length);

  bool decode_header();
  void encode_header();

private:
  char data_[header_length + max_body_length];
  size_t body_length_;
};

#endif /* end of include guard: _MESSAGE_H_ */

