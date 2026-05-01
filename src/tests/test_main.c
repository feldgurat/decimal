#include <check.h>
#include <stdlib.h>

Suite* suite_add(void);
Suite* suite_sub(void);
Suite* suite_mul(void);
Suite* suite_div(void);
Suite* suite_is_less(void);
Suite* suite_is_less_or_equal(void);
Suite* suite_is_greater(void);
Suite* suite_is_greater_or_equal(void);
Suite* suite_is_equal(void);
Suite* suite_is_not_equal(void);
Suite* suite_from_int_to_decimal(void);
Suite* suite_from_float_to_decimal(void);
Suite* suite_from_decimal_to_int(void);
Suite* suite_from_decimal_to_float(void);
Suite* suite_floor(void);
Suite* suite_round(void);
Suite* suite_truncate(void);
Suite* suite_negate(void);

int main(void) {
  int failed = 0;
  SRunner* sr = srunner_create(suite_from_int_to_decimal());
  srunner_add_suite(sr, suite_add());
  srunner_add_suite(sr, suite_sub());
  srunner_add_suite(sr, suite_mul());
  srunner_add_suite(sr, suite_div());
  srunner_add_suite(sr, suite_is_less());
  srunner_add_suite(sr, suite_is_less_or_equal());
  srunner_add_suite(sr, suite_is_greater());
  srunner_add_suite(sr, suite_is_greater_or_equal());
  srunner_add_suite(sr, suite_is_equal());
  srunner_add_suite(sr, suite_is_not_equal());
  srunner_add_suite(sr, suite_from_float_to_decimal());
  srunner_add_suite(sr, suite_from_decimal_to_int());
  srunner_add_suite(sr, suite_from_decimal_to_float());
  srunner_add_suite(sr, suite_floor());
  srunner_add_suite(sr, suite_round());
  srunner_add_suite(sr, suite_truncate());
  srunner_add_suite(sr, suite_negate());

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}