/*
 * @Author: mike mike955@163.cn
 * @Date: 2023-05-05 09:39:59
 * @LastEditors: mike
 * @LastEditTime: 2023-05-05 10:04:25
 * @FilePath: /mindb/src/include/mindb/db.h
 */
#ifndef SRC_INCLUDE_MINDB_DB_H_
#define SRC_INCLUDE_MINDB_DB_H_

#include "mindb/options.h"
#include "mindb/status.h"
namespace mindb {

struct Options;
struct ReadOptions;
struct WriteOptions;
class WriteBatch;

class DB {
 public:
  static Status Open(const Options& options, const std::string& dbname,
                     DB** dbptr);
  DB() = default;

  DB(const DB&) = delete;
  DB& operator=(const DB&) = delete;
  virtual ~DB();
};

}  // namespace mindb

#endif  // SRC_INCLUDE_MINDB_DB_H_