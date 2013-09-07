#include "message.h"

void Message::body_length(size_t new_length) {
  body_length_ = new_length;
  if (body_length_ > max_body_length)
    body_length_ = max_body_length;
}

bool Message::decode_header() {
  using namespace std; // For strncat and atoi.
  char header[header_length + 1] = "";
  strncat(header, data_, header_length);
  body_length_ = atoi(header);
  if (body_length_ > max_body_length)
  {
    body_length_ = 0;
    return false;
  }
  return true;
}

void Message::encode_header()
{
  using namespace std; // For sprintf and memcpy.
  char header[header_length + 1] = "";
  sprintf(header, "%4zd", body_length_);
  memcpy(data_, header, header_length);
}
