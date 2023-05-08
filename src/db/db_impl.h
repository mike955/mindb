/*
 * @Author: mike mike955@163.cn
 * @Date: 2023-05-05 10:11:27
 * @LastEditors: mike
 * @LastEditTime: 2023-05-05 10:11:31
 * @FilePath: /mindb/src/db/db_impl.h
 */
#ifndef SRC_DB_DB_IMPL_H_
#define SRC_DB_DB_IMPL_H_

#include <atomic>
#include <deque>
#include <set>
#include <string>

#include "db/db_format.h"
#include "db/table_cache.h"
#include "db/version_edit.h"
#include "db/version_set.h"
#include "mindb/db.h"
#include "mindb/env.h"
#include "mindb/options.h"
#include "mindb/status.h"
#include "port/port.h"
#include "db/builder.h"

namespace mindb {

class MemTable;
class TableCache;
class Version;
class VersionEdit;
class VersionSet;

class DBImpl : public DB {
 public:
  DBImpl(const Options& options, const std::string& dbname);
  DBImpl(const DBImpl&) = delete;
  DBImpl& operator=(const DBImpl&) = delete;
  ~DBImpl() override;

 private:
  friend class DB;

  Status NewDB();
  Status Recover(VersionEdit* edit, bool* save_manifest)
      EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  Status RecoverLogFile(uint64_t log_number, bool last_log, bool* save_manifest,
                        VersionEdit* edit, SequenceNumber* max_sequence)
      EXCLUSIVE_LOCKS_REQUIRED(mutex_);
  const Comparator* user_comparator() const {
    return internal_comparator_.user_comparator();
  }
  Status WriteLevel0Table(MemTable* mem, VersionEdit* edit, Version* base)
      EXCLUSIVE_LOCKS_REQUIRED(mutex_);

  Env* const env_;
  const InternalKeyComparator internal_comparator_;
  const Options options_;  // options_.comparator == &internal_comparator_
  const std::string dbname_;
  TableCache* const table_cache_;
  FileLock* db_lock_;
  port::Mutex mutex_;
  MemTable* mem_;
  MemTable* imm_ GUARDED_BY(mutex_);  // Memtable being compacted
  std::atomic<bool> has_imm_;         // So bg thread can detect non-null imm_
  WritableFile* logfile_;
  uint64_t logfile_number_ GUARDED_BY(mutex_);
  VersionSet* const versions_ GUARDED_BY(mutex_);

  std::set<uint64_t> pending_outputs_ GUARDED_BY(mutex_);
};

Options SanitizeOptions(const std::string& db,
                        const InternalKeyComparator* icmp,
                        // const InternalFilterPolicy* ipolicy,
                        const Options& src);

}  // namespace mindb

#endif  // SRC_DB_DB_IMPL_H_