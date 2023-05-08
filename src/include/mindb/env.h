
#ifndef SRC_INCLUDE_MINDB_ENV_H_
#define SRC_INCLUDE_MINDB_ENV_H_

#include <vector>

#include "mindb/status.h"

namespace mindb {
class SequentialFile;

class FileLock {
 public:
  FileLock() = default;

  FileLock(const FileLock&) = delete;
  FileLock& operator=(const FileLock&) = delete;

  virtual ~FileLock();
};

class RandomAccessFile {
 public:
  RandomAccessFile() = default;

  RandomAccessFile(const RandomAccessFile&) = delete;
  RandomAccessFile& operator=(const RandomAccessFile&) = delete;

  virtual ~RandomAccessFile();

  // Read up to "n" bytes from the file starting at "offset".
  // "scratch[0..n-1]" may be written by this routine.  Sets "*result"
  // to the data that was read (including if fewer than "n" bytes were
  // successfully read).  May set "*result" to point at data in
  // "scratch[0..n-1]", so "scratch[0..n-1]" must be live when
  // "*result" is used.  If an error was encountered, returns a non-OK
  // status.
  //
  // Safe for concurrent use by multiple threads.
  virtual Status Read(uint64_t offset, size_t n, Slice* result,
                      char* scratch) const = 0;
};

class WritableFile {
 public:
  WritableFile() = default;

  WritableFile(const WritableFile&) = delete;
  WritableFile& operator=(const WritableFile&) = delete;

  virtual ~WritableFile();

  virtual Status Append(const Slice& data) = 0;
  virtual Status Close() = 0;
  virtual Status Flush() = 0;
  virtual Status Sync() = 0;
};

class Env {
 public:
  Env();

  Env(const Env&) = delete;
  Env& operator=(const Env&) = delete;
  virtual ~Env();
  static Env* Default();

  virtual Status RemoveFile(const std::string& fname);
  virtual Status DeleteFile(const std::string& fname);
  virtual Status CreateDir(const std::string& dirname) = 0;
  virtual Status RemoveDir(const std::string& dirname);
  virtual Status DeleteDir(const std::string& dirname);

  virtual Status LockFile(const std::string& fname, FileLock** lock) = 0;
  virtual Status UnlockFile(FileLock* lock) = 0;
  virtual Status RenameFile(const std::string& src,
                            const std::string& target) = 0;
  virtual bool FileExists(const std::string& fname) = 0;

  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) = 0;
  virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) = 0;
  virtual Status NewAppendableFile(const std::string& fname,
                                   WritableFile** result);
  virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) = 0;
  virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result) = 0;
  virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) = 0;
};

class SequentialFile {
 public:
  SequentialFile() = default;

  SequentialFile(const SequentialFile&) = delete;
  SequentialFile& operator=(const SequentialFile&) = delete;

  virtual ~SequentialFile();

  // Read up to "n" bytes from the file.  "scratch[0..n-1]" may be
  // written by this routine.  Sets "*result" to the data that was
  // read (including if fewer than "n" bytes were successfully read).
  // May set "*result" to point at data in "scratch[0..n-1]", so
  // "scratch[0..n-1]" must be live when "*result" is used.
  // If an error was encountered, returns a non-OK status.
  //
  // REQUIRES: External synchronization
  virtual Status Read(size_t n, Slice* result, char* scratch) = 0;

  // Skip "n" bytes from the file. This is guaranteed to be no
  // slower that reading the same data, but may be faster.
  //
  // If end of file is reached, skipping will stop at the end of the
  // file, and Skip will return OK.
  //
  // REQUIRES: External synchronization
  virtual Status Skip(uint64_t n) = 0;
};

Status WriteStringToFile(Env* env, const Slice& data, const std::string& fname);

// A utility routine: read contents of named file into *data
Status ReadFileToString(Env* env, const std::string& fname, std::string* data);

}  // namespace mindb

#endif  // SRC_INCLUDE_MINDB_ENV_H_