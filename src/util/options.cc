#include "mindb/options.h"
#include "mindb/comparator.h"
#include "mindb/env.h"

namespace mindb {
Options::Options() : comparator(BytewiseComparator()), env(Env::Default()) {}
}  // namespace mindb