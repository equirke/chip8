#include <check.h>
#include <stdlib.h>
#include "parse_instruction.h"

START_TEST(test_instruction_parse)
{
    ck_assert_int_eq(parse_opcode(0x75, 0x32), ADD_RC);
    ck_assert_int_eq(get_v_reg_x(0x75), 5);
    ck_assert_int_eq(get_v_reg_y(0xA8), 10);
}
END_TEST

Suite* parse_instruction_suite(void)
{
    Suite* s;
    TCase* tcase;

    s = suite_create("Chip8 Parse instructions");
    tcase = tcase_create("Main");
    tcase_add_test(tcase, test_instruction_parse);
    suite_add_tcase(s, tcase);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = parse_instruction_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}