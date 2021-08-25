#include "array/zfpcpp.h"
using namespace zfp;

extern "C" {
  #include "constants/3dFloat.h"
  #include "utils/rand32.h"
}

#define ZFP_FIELD_FUNC zfp_field_3d
#define ZFP_ENCODE_BLOCK_FUNC zfp_encode_block_float_3
#define ZFP_DECODE_BLOCK_FUNC zfp_decode_block_float_3
#define ZFP_ENCODE_BLOCK_STRIDED_FUNC zfp_encode_block_strided_float_3
#define ZFP_DECODE_BLOCK_STRIDED_FUNC zfp_decode_block_strided_float_3
#define ZFP_ENCODE_PARTIAL_BLOCK_STRIDED_FUNC zfp_encode_partial_block_strided_float_3
#define ZFP_DECODE_PARTIAL_BLOCK_STRIDED_FUNC zfp_decode_partial_block_strided_float_3

#define SCALAR float
#define DIMS 3

#include "testTemplatedDecodeBase.cpp"
