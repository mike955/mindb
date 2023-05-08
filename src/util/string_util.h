#ifndef SRC_UTIL_STRING_UTIL_H_
#define SRC_UTIL_STRING_UTIL_H_

#include "mindb/slice.h"

namespace mindb {

extern void AppendEscapedStringTo(std::string* str, const Slice& value);

extern std::string EscapeString(const Slice& value);

}

#endif  // SRC_UTIL_STRING_UTIL_H_