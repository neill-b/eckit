/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/runtime/Context.h"
#include "eckit/exception/Exceptions.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/config/ResourceFromFiles.h"
#include "eckit/mpi/ParallelContextBehavior.h"
#include "eckit/mpi/mpi.h"
#include "eckit/mpi/BroadcastFile.h"

namespace eckit {
namespace mpi {

//-----------------------------------------------------------------------------

struct ParallelReadPolicy
{
  int root; // master task that reads

  /// @return true if file was read
  friend bool read(const ParallelReadPolicy& policy, const PathName& path, std::stringstream& stream )
  {
    return mpi::broadcast_file(path,stream,policy.root);
  }

};

//-----------------------------------------------------------------------------

static void parallel_abort() {
    MPI_Abort(eckit::mpi::world(), -1);
}

ParallelContextBehavior::ParallelContextBehavior() :
  StandardBehavior()
{
  mpi::Environment::instance();

  eckit::Context::instance().abortHandler(&parallel_abort);
}

FileReadPolicy ParallelContextBehavior::fileReadPolicy()
{
  ParallelReadPolicy mypolicy;
  mypolicy.root = 0;
  return mypolicy;
}

//-----------------------------------------------------------------------------

} // namespace mpi
} // namespace eckit

