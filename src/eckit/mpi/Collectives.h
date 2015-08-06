/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef ECKIT_MPI_COLLECTIVES_h
#define ECKIT_MPI_COLLECTIVES_h

#include "eckit/mpi/mpi.h"
#include "eckit/mpi/Exceptions.h"

namespace eckit {
namespace mpi {

/// @brief Buffer
///
/// Class that keeps allocation of a MPI buffer including
/// counts and displacements
template <typename DATA_TYPE> struct Buffer;

// ----------------------------------------------------------------------------------

template<typename DATA_TYPE>
inline int broadcast( const Comm& comm,
                      DATA_TYPE& data,
                      size_t root );

template<typename DATA_TYPE>
inline int broadcast( DATA_TYPE& data,
                      size_t root );

template<typename DATA_TYPE>
inline int broadcast( const Comm& comm,
                      std::vector<DATA_TYPE>& data,
                      size_t root );

template<typename DATA_TYPE>
inline int broadcast( std::vector<DATA_TYPE>& data,
                      size_t root );

template<typename DATA_TYPE>
inline int all_reduce( const Comm& comm,
                       const DATA_TYPE& send,
                             DATA_TYPE& recv,
                       const Operation& );

template<typename DATA_TYPE>
inline int all_reduce( const DATA_TYPE& send,
                             DATA_TYPE& recv,
                       const Operation& );

template<typename DATA_TYPE>
inline int all_reduce( const Comm& comm,
                       const std::vector<DATA_TYPE>& send,
                             std::vector<DATA_TYPE>& recv,
                       const Operation& );

template<typename DATA_TYPE>
inline int all_reduce( const std::vector<DATA_TYPE>& send,
                             std::vector<DATA_TYPE>& recv,
                       const Operation& );

template< typename DATA_TYPE >
inline int all_to_all( const Comm& comm,
                       const std::vector< std::vector<DATA_TYPE> >& sendvec,
                             std::vector< std::vector<DATA_TYPE> >& recvvec );


template< typename DATA_TYPE >
inline int all_to_all( const std::vector< std::vector<DATA_TYPE> >& sendvec,
                             std::vector< std::vector<DATA_TYPE> >& recvvec );


template<typename DATA_TYPE>
inline int all_gather( const Comm& comm,
                       const DATA_TYPE send[], int sendcnt,
                       Buffer<DATA_TYPE>& recv );


template<typename DATA_TYPE>
inline int all_gather( const DATA_TYPE send[], int sendcnt,
                       Buffer<DATA_TYPE>& recv );


template<typename VECTOR>
inline int all_gather( const Comm& comm,
                       const VECTOR& send,
                       Buffer<typename VECTOR::value_type>& recv );


template<typename VECTOR>
inline int all_gather( const VECTOR& send,
                       Buffer<typename VECTOR::value_type>& recv );


// ----------------------------------------------------------------------------------

template <typename DATA_TYPE>
struct Buffer
{
  typedef DATA_TYPE value_type;
  typedef typename std::vector<DATA_TYPE>::iterator iterator;
  int                    cnt;
  std::vector<int>       counts;
  std::vector<int>       displs;
  std::vector<DATA_TYPE> buf;

  Buffer()
  {
    counts.resize( eckit::mpi::size() );
    displs.resize( eckit::mpi::size() );
  }

  iterator begin() { return buf.begin(); }
  iterator end()   { return buf.end();   }

};

// ----------------------------------------------------------------------------------

template<typename DATA_TYPE>
inline int broadcast( const Comm& comm,
                      DATA_TYPE& data,
                      size_t root )
{
  ECKIT_MPI_CHECK_RESULT( MPI_Bcast(&data,1,eckit::mpi::datatype<DATA_TYPE>(),int(root),comm) );
  return MPI_SUCCESS;
}

template<typename DATA_TYPE>
inline int broadcast( DATA_TYPE& data,
                      size_t root )
{
  return broadcast(comm(),data,root);
}

template<typename DATA_TYPE>
inline int broadcast( const Comm& comm,
                      std::vector<DATA_TYPE>& data,
                      size_t root )
{
  ECKIT_MPI_CHECK_RESULT( MPI_Bcast(data.data(),int(data.size()),eckit::mpi::datatype<DATA_TYPE>(),int(root),comm) );
  return MPI_SUCCESS;
}

template<typename DATA_TYPE>
inline int broadcast( std::vector<DATA_TYPE>& data,
                      size_t root )
{
  return broadcast(comm(),data,root);
}

template<typename DATA_TYPE>
inline int all_reduce( const Comm& comm,
                       const DATA_TYPE& send,
                             DATA_TYPE& recv,
                       const Operation& mpi_op )
{
  ECKIT_MPI_CHECK_RESULT( MPI_Allreduce(&const_cast<DATA_TYPE&>(send),&recv,1,datatype<DATA_TYPE>(),mpi_op,comm) );
  return MPI_SUCCESS;
}

template<typename DATA_TYPE>
inline int all_reduce( const DATA_TYPE& send,
                             DATA_TYPE& recv,
                       const Operation& mpi_op )
{
  return all_reduce(comm(),send,recv,mpi_op);
}

template<typename DATA_TYPE>
inline int all_reduce( const Comm& comm,
                       const std::vector<DATA_TYPE>& send,
                             std::vector<DATA_TYPE>& recv,
                       const Operation& mpi_op )
{
  recv.resize(send.size());
  ECKIT_MPI_CHECK_RESULT( MPI_Allreduce(const_cast<DATA_TYPE*>(send.data()),recv.data(),send.size(),datatype<DATA_TYPE>(),mpi_op,comm) );
  return MPI_SUCCESS;
}

template<typename DATA_TYPE>
inline int all_reduce( const std::vector<DATA_TYPE>& send,
                             std::vector<DATA_TYPE>& recv,
                       const Operation& mpi_op )
{
  return all_reduce(comm(),send,recv,mpi_op);
}

template< typename DATA_TYPE >
inline int all_to_all( const Comm& comm,
                       const std::vector< std::vector<DATA_TYPE> >& sendvec,
                             std::vector< std::vector<DATA_TYPE> >& recvvec )
{
  int cnt;
  int mpi_size = comm.size();
  ASSERT( sendvec.size() == mpi_size );
  ASSERT( recvvec.size() == mpi_size );
  // Get send-information
  std::vector<int> sendcounts(mpi_size);
  std::vector<int> senddispls(mpi_size);
  int sendcnt;
  senddispls[0] = 0;
  sendcounts[0] = sendvec[0].size();
  sendcnt = sendcounts[0];
  for( int jproc=1; jproc<mpi_size; ++jproc )
  {
    senddispls[jproc] = senddispls[jproc-1] + sendcounts[jproc-1];
    sendcounts[jproc] = sendvec[jproc].size();
    sendcnt += sendcounts[jproc];
  }


  // Get recv-information
  std::vector<int> recvcounts(mpi_size);
  std::vector<int> recvdispls(mpi_size);
  int recvcnt;
  ECKIT_MPI_CHECK_RESULT( MPI_Alltoall( sendcounts.data(), 1, MPI_INT,
                                  recvcounts.data(), 1, MPI_INT,
                                  comm ) );
  recvdispls[0] = 0;
  recvcnt = recvcounts[0];
  for( int jproc=1; jproc<mpi_size; ++jproc )
  {
    recvdispls[jproc] = recvdispls[jproc-1] + recvcounts[jproc-1];
    recvcnt += recvcounts[jproc];
  }

  // Communicate
  std::vector<DATA_TYPE> sendbuf(sendcnt);
  std::vector<DATA_TYPE> recvbuf(recvcnt);
  cnt = 0;
  for( int jproc=0; jproc<mpi_size; ++jproc )
  {
    for( int i=0; i<sendcounts[jproc]; ++i )
    {
      sendbuf[cnt++] = sendvec[jproc][i];
    }
  }
  ECKIT_MPI_CHECK_RESULT( MPI_Alltoallv(
                      sendbuf.data(), sendcounts.data(), senddispls.data(), eckit::mpi::datatype<DATA_TYPE>(),
                      recvbuf.data(), recvcounts.data(), recvdispls.data(), eckit::mpi::datatype<DATA_TYPE>(),
                      comm ) );
  cnt=0;
  for( int jproc=0; jproc<mpi_size; ++jproc )
  {
    recvvec[jproc].resize(recvcounts[jproc]);
    for( int i=0; i<recvcounts[jproc]; ++i )
    {
      recvvec[jproc][i] = recvbuf[cnt++];
    }
  }
  return MPI_SUCCESS;
}

template< typename DATA_TYPE >
inline int all_to_all( const std::vector< std::vector<DATA_TYPE> >& sendvec,
                             std::vector< std::vector<DATA_TYPE> >& recvvec )
{
  return all_to_all( comm(), sendvec, recvvec );
}


template<typename DATA_TYPE>
inline int all_gather( const Comm& comm, const DATA_TYPE send[], int sendcnt, Buffer<DATA_TYPE>& recv )
{
  int mpi_size = comm.size();
  ECKIT_MPI_CHECK_RESULT( MPI_Allgather( &sendcnt,           1, MPI_INT,
                                         recv.counts.data(), 1, MPI_INT, comm ) );
  recv.displs[0] = 0;
  recv.cnt = recv.counts[0];
  for( int jpart=1; jpart<mpi_size; ++jpart )
  {
    recv.displs[jpart] = recv.displs[jpart-1] + recv.counts[jpart-1];
    recv.cnt += recv.counts[jpart];
  }
  recv.buf.resize(recv.cnt);

  ECKIT_MPI_CHECK_RESULT( MPI_Allgatherv( const_cast<DATA_TYPE*>(send), sendcnt, eckit::mpi::datatype<DATA_TYPE>(),
                                          recv.buf.data(), recv.counts.data(), recv.displs.data(),
                                          eckit::mpi::datatype<DATA_TYPE>(), comm ) );
  return MPI_SUCCESS;
}
template<typename DATA_TYPE>
inline int all_gather( const DATA_TYPE send[], int sendcnt, Buffer<DATA_TYPE>& recv )
{
  return all_gather( comm(), send, sendcnt, recv );
}

template<typename VECTOR>
inline int all_gather( const Comm& comm, const VECTOR& send, Buffer<typename VECTOR::value_type>& recv )
{
  return all_gather( comm, send.data(), send.size(), recv );
}

template<typename VECTOR>
inline int all_gather( const VECTOR& send, Buffer<typename VECTOR::value_type>& recv )
{
  return all_gather( comm(), send, recv );
}


template<typename VECTOR>
inline int all_gather( const Comm& comm, const typename VECTOR::value_type& send, VECTOR& recv )
{
  typedef typename VECTOR::value_type DATA_TYPE;
  const int mpi_size = comm.size();
  const int sendcnt = 1;
  std::vector<int> displs(mpi_size);
  std::vector<int> counts(mpi_size,1);
  for( int jpart=0; jpart<mpi_size; ++jpart )
    displs[jpart] = jpart;
  recv.resize(mpi_size);
  ECKIT_MPI_CHECK_RESULT( MPI_Allgatherv( const_cast<DATA_TYPE*>(&send), sendcnt, eckit::mpi::datatype<DATA_TYPE>(),
                          recv.data(), counts.data(), displs.data(),
                          eckit::mpi::datatype<DATA_TYPE>(), comm ) );
  return MPI_SUCCESS;
}

template<typename VECTOR>
inline int all_gather( const typename VECTOR::value_type& send, VECTOR& recv )
{
  return all_gather( comm(), send, recv );
}

} // namespace mpi
} // namepsace eckit

#endif // ECKIT_MPI_COLLECTIVES_h
