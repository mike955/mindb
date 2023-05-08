#ifndef SRC_DB_TABLE_CACHE_H_
#define SRC_DB_TABLE_CACHE_H_

#include <string>

#include "mindb/cache.h"
#include "mindb/iterator.h"
#include "mindb/options.h"
#include "mindb/table.h"

namespace mindb {
class TableCache {
 public:
  TableCache(const std::string& dbname, const Options& options, int entries);

  TableCache(const TableCache&) = delete;
  TableCache& operator=(const TableCache&) = delete;

  ~TableCache();

  Iterator* NewIterator(const ReadOptions& options, uint64_t file_number,
                        uint64_t file_size, Table** tableptr = nullptr);

 private:
  Status FindTable(uint64_t file_number, uint64_t file_size, Cache::Handle**);

  Env* const env_;
  const std::string dbname_;
  const Options& options_;
  Cache* cache_;
};
}  // namespace mindb

#endif  // SRC_DB_TABLE_CACHE_H_