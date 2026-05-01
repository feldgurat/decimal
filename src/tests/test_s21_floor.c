#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_floor_positive_fraction) {
  s21_decimal val = {{1237, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_floor(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(123, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal val = {{1237, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_floor(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-124, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_floor_positive_integer) {
  s21_decimal val;
  s21_from_int_to_decimal(50, &val);

  s21_decimal res;
  s21_floor(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
}
END_TEST

START_TEST(test_floor_negative_integer) {
  s21_decimal val;
  s21_from_int_to_decimal(-50, &val);

  s21_decimal res;
  s21_floor(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
}
END_TEST

START_TEST(test_floor_less_than_one) {
  s21_decimal val = {{3, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_floor(val, &res);

  s21_decimal expected = S21_DECIMAL_ZERO;

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
}
END_TEST

START_TEST(test_floor_negative_less_than_one) {
  s21_decimal val = {{3, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_floor(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-1, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
}
END_TEST

START_TEST(test_floor_unnormilize_decimal) {
  s21_decimal val = {{50, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_floor(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-5, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal val = S21_DECIMAL_ZERO;
  s21_decimal res;

  s21_floor(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
}
END_TEST

START_TEST(test_floor_null) {
  s21_decimal val;
  s21_from_int_to_decimal(10, &val);

  int err = s21_floor(val, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_floor_negative_point_one) {
  s21_decimal val = {{1, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);
  s21_decimal res;
  s21_floor(val, &res);
  ck_assert_uint_eq(res.bits[0], 1);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_floor_positive_point_nine) {
  s21_decimal val = {{9, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_decimal res;
  s21_floor(val, &res);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_floor_negative_exact) {
  s21_decimal val = {{30, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);
  s21_decimal res;
  s21_floor(val, &res);
  ck_assert_uint_eq(res.bits[0], 3);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

Suite* suite_floor(void) {
  Suite* s = suite_create("suite_floor");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_floor_positive_fraction);
  tcase_add_test(tc, test_floor_negative_fraction);
  tcase_add_test(tc, test_floor_positive_integer);
  tcase_add_test(tc, test_floor_negative_integer);
  tcase_add_test(tc, test_floor_less_than_one);
  tcase_add_test(tc, test_floor_negative_less_than_one);
  tcase_add_test(tc, test_floor_zero);
  tcase_add_test(tc, test_floor_null);
  tcase_add_test(tc, test_floor_unnormilize_decimal);

  tcase_add_test(tc, test_floor_negative_point_one);
  tcase_add_test(tc, test_floor_positive_point_nine);
  tcase_add_test(tc, test_floor_negative_exact);

  suite_add_tcase(s, tc);
  return s;
}