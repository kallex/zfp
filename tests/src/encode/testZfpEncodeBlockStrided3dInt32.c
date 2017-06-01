#include "src/encode3i.c"

#include "constants/3dInt32.h"
#include "utils/testMacros.h"
#include "utils/rand32.c"
#include "utils/hash32.c"
#include "zfpEncodeBlockStridedBase.c"

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(when_seededRandomDataGenerated_expect_ChecksumMatches, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodeBlockStrided_expect_ReturnValReflectsNumBitsWrittenToBitstream, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodeBlockStrided_expect_OnlyStridedEntriesUsed, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodeBlockStrided_expect_BitstreamChecksumMatches, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodePartialBlockStrided_expect_ReturnValReflectsNumBitsWrittenToBitstream, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodePartialBlockStrided_expect_OnlyStridedEntriesUsed, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodePartialBlockStrided_expect_OnlyEntriesWithinPartialBlockBoundsUsed, setup, teardown),
    cmocka_unit_test_setup_teardown(given_3dInt32Block_when_EncodePartialBlockStrided_expect_BitstreamChecksumMatches, setup, teardown),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
