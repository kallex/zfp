#include "src/decode3l.c"

#include "constants/3dInt64.h"
#include "utils/testMacros.h"
#include "utils/rand64.c"
#include "utils/hash64.c"
#include "zfpDecodeBlockStridedBase.c"

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(when_seededRandomDataGenerated_expect_ChecksumMatches, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodeBlockStrided_expect_ReturnValReflectsNumBitsReadFromBitstream, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodeBlockStrided_expect_OnlyStridedEntriesChangedInDestinationArray, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodeBlockStrided_expect_ArrayChecksumMatches, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodePartialBlockStrided_expect_ReturnValReflectsNumBitsReadFromBitstream, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodePartialBlockStrided_expect_NonStridedEntriesUnchangedInDestinationArray, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodePartialBlockStrided_expect_EntriesOutsidePartialBlockBoundsUnchangedInDestinationArray, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt64Block_when_DecodePartialBlockStrided_expect_ArrayChecksumMatches, setup, teardown),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
