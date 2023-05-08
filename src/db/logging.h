#ifndef SRC_DB_LOGGING_H_
#define SRC_DB_LOGGING_H_

#include <cstdint>
#include <cstdio>
#include <string>

#include "mindb/slice.h"

namespace mindb {

bool ConsumeDecimalNumber(Slice* in, uint64_t* val);
}

#endif  // SRC_DB_LOGGING_H_