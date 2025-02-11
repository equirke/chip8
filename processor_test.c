#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "processor.h"
#include "chip8.h"
#include "chip_constants.h"

START_TEST(test_add_no_carry)
{
    chip8 chip;
    chip.PC = 0;
    chip.v_regs[1] = 5;
    chip.v_regs[0] = 3;
    chip.v_regs[REG_VF] = 0;

    op_add_reg_to_reg(&chip, 1, 0);
    ck_assert_uint_eq(chip.v_regs[1], 8);
    ck_assert_uint_eq(chip.v_regs[REG_VF], 0);
}
END_TEST

START_TEST(test_add_carry)
{
    chip8 chip;
    chip.PC = 0;
    chip.v_regs[1] = 160;
    chip.v_regs[0] = 170;
    chip.v_regs[REG_VF] = 0;

    op_add_reg_to_reg(&chip, 1, 0);
    ck_assert_uint_eq(chip.v_regs[1], 74);
    ck_assert_uint_eq(chip.v_regs[REG_VF], 1);
}
END_TEST

Suite* processor_suite(void)
{
    Suite* s;
    TCase* tcase;

    s = suite_create("Chip8 Processor");
    tcase = tcase_create("Main");
    tcase_add_test(tcase, test_add_no_carry);
    tcase_add_test(tcase, test_add_carry);
    suite_add_tcase(s, tcase);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = processor_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}