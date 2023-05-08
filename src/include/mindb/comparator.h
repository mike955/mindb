#ifndef SRC_INCLUDE_MINDB_COMPARATOR_H_
#define SRC_INCLUDE_MINDB_COMPARATOR_H_

#include <string>

namespace mindb {

class Slice;

class Comparator {
 public:
  virtual ~Comparator();
  virtual int Compare(const Slice& a, const Slice& b) const = 0;
  virtual const char* Name() const = 0;
  virtual void FindShortestSeparator(std::string* start,
                                     const Slice& limit) const = 0;

  // Changes *key to a short string >= *key.
  // Simple comparator implementations may return with *key unchanged,
  // i.e., an implementation of this method that does nothing is correct.
  virtual void FindShortSuccessor(std::string* key) const = 0;
};

const Comparator* BytewiseComparator();

}  // namespace mindb

#endif  // SRC_INCLUDE_MINDB_COMPARATOR_H_