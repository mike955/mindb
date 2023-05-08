#ifndef SRC_INCLUDE_MINDB_OPTIONS_H_
#define SRC_INCLUDE_MINDB_OPTIONS_H_

#include <cstddef>

#include "mindb/comparator.h"

namespace mindb {
class Env;
class Cache;
class FilterPolicy;

enum CompressionType {
  kNoCompression = 0x0,
  kSnappyCompression = 0x1,
  kZstdCompression = 0x2,
};

struct Options {
  Options();
  const Comparator* comparator;
  bool create_if_missing = false;

  bool error_if_exists = false;
  Env* env;
  // Logger* info_log = nullptr;
  size_t write_buffer_size = 4 * 1024 * 1024;
  int max_open_files = 1000;
  Cache* block_cache = nullptr;
  bool paranoid_checks = false;
  // Cache* block_cache = nullptr;
  size_t block_size = 4 * 1024;
  int block_restart_interval = 16;
  size_t max_file_size = 2 * 1024 * 1024;
  CompressionType compression = kSnappyCompression;
  int zstd_compression_level = 1;
  bool reuse_logs = false;
  const FilterPolicy* filter_policy = nullptr;
};

struct ReadOptions {
  bool verify_checksums = false;
  bool fill_cache = true;
};

struct WriteOptions {
  WriteOptions() = default;
  bool sync = false;
};

}  // namespace mindb

#endif  // SRC_INCLUDE_MINDB_OPTIONS_H_