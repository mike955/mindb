#ifndef SRC_DB_VERSION_SET_H_
#define SRC_DB_VERSION_SET_H_

#include <string>
#include <vector>

#include "db/db_format.h"
#include "db/log_writer.h"
#include "db/table_cache.h"
#include "db/version_edit.h"
#include "mindb/env.h"
#include "mindb/options.h"
#include "mindb/status.h"

namespace mindb {
class VersionSet;

bool SomeFileOverlapsRange(const InternalKeyComparator& icmp,
                           bool disjoint_sorted_files,
                           const std::vector<FileMetaData*>& files,
                           const Slice* smallest_user_key,
                           const Slice* largest_user_key);

class Version {
 public:
  void Ref();
  void Unref();

  void GetOverlappingInputs(
      int level,
      const InternalKey* begin,  // nullptr means before all keys
      const InternalKey* end,    // nullptr means after all keys
      std::vector<FileMetaData*>* inputs);
  bool OverlapInLevel(int level, const Slice* smallest_user_key,
                      const Slice* largest_user_key);
  int PickLevelForMemTableOutput(const Slice& smallest_user_key,
                                 const Slice& largest_user_key);

 private:
  friend class VersionSet;
  explicit Version(VersionSet* vset)
      : vset_(vset),
        next_(this),
        prev_(this),
        refs_(0),
        // file_to_compact_(nullptr),
        file_to_compact_level_(-1),
        compaction_score_(-1),
        compaction_level_(-1) {}

  Version(const Version&) = delete;
  Version& operator=(const Version&) = delete;

  ~Version();

  VersionSet* vset_;  // VersionSet to which this Version belongs
  Version* next_;     // Next version in linked list
  Version* prev_;     // Previous version in linked list
  int refs_;          // Number of live refs to this version

  std::vector<FileMetaData*> files_[config::kNumLevels];
  int file_to_compact_level_;

  // Level that should be compacted next and its compaction score.
  // Score < 1 means compaction is not strictly needed.  These fields
  // are initialized by Finalize().
  double compaction_score_;
  int compaction_level_;
};

class VersionSet {
 public:
  VersionSet(const std::string& dbname, const Options* options,
             TableCache* table_cache, const InternalKeyComparator*);
  VersionSet(const VersionSet&) = delete;
  VersionSet& operator=(const VersionSet&) = delete;

  ~VersionSet();

  Status Recover(bool* save_manifest);
  uint64_t LastSequence() const { return last_sequence_; }

  void SetLastSequence(uint64_t s) {
    assert(s >= last_sequence_);
    last_sequence_ = s;
  }
  void MarkFileNumberUsed(uint64_t number);
  uint64_t LogNumber() const { return log_number_; }
  uint64_t PrevLogNumber() const { return prev_log_number_; }
  void AddLiveFiles(std::set<uint64_t>* live);
  uint64_t NewFileNumber() { return next_file_number_++; }

 private:
  class Builder;
  friend class Version;

  bool ReuseManifest(const std::string& dscname, const std::string& dscbase);
  void Finalize(Version* v);
  void AppendVersion(Version* v);

  Env* const env_;
  const std::string dbname_;
  const Options* const options_;
  const InternalKeyComparator icmp_;
  uint64_t next_file_number_;
  uint64_t manifest_file_number_;
  uint64_t last_sequence_;
  uint64_t log_number_;
  uint64_t prev_log_number_;

  WritableFile* descriptor_file_;
  log::Writer* descriptor_log_;
  Version dummy_versions_;
  Version* current_;
  std::string compact_pointer_[config::kNumLevels];
};

}  // namespace mindb

#endif  // SRC_DB_VERSION_SET_H_