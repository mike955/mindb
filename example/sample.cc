#include <cassert>

#include "mindb/db.h"
#include "mindb/options.h"

using mindb::DB;
using mindb::Options;
using mindb::Status;

std::string kDBPath = "/tmp/testdb";

int main() {
  DB* db;
  Options options;
  // options.create_if_missing = true;
  Status status = DB::Open(options, kDBPath, &db);
  // assert(status.ok());
}