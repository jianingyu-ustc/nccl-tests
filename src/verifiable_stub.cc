/*************************************************************************
 * KLX path stub for nccl-tests verifiable kernels.
 ************************************************************************/

#include "../verifiable/verifiable.h"

cudaError_t ncclVerifiablePrepareInput(
    void* /*elts*/, intptr_t /*elt_n*/, int /*elt_ty*/, int /*red_op*/,
    int /*rank_n*/, int /*rank_me*/, uint64_t /*seed*/, intptr_t /*elt_ix0*/,
    cudaStream_t /*stream*/) {
  return cudaSuccess;
}

cudaError_t ncclVerifiablePrepareExpected(
    void* /*elts*/, intptr_t /*elt_n*/, int /*elt_ty*/, int /*red_op*/,
    int /*rank_n*/, uint64_t /*seed*/, intptr_t /*elt_ix0*/,
    cudaStream_t /*stream*/) {
  return cudaSuccess;
}

cudaError_t ncclVerifiableVerify(
    void const* /*results*/, void const* /*expected*/, intptr_t /*elt_n*/,
    int /*elt_ty*/, int /*red_op*/, int /*rank_n*/, uint64_t /*seed*/,
    intptr_t /*elt_ix0*/, int64_t* bad_elt_n, cudaStream_t /*stream*/) {
  if (bad_elt_n != nullptr) {
    *bad_elt_n = 0;
  }
  return cudaSuccess;
}

#ifdef NCCL_VERIFIABLE_SELF_TEST
void ncclVerifiableLaunchSelfTest() {}
#endif
