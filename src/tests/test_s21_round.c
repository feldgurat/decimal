#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_round_integer) {
  s21_decimal val, res;
  s21_from_int_to_decimal(123, &val);

  int err = s21_round(val, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_down) {
  s21_decimal val = {{1234, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(123, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_half_up) {
  s21_decimal val = {{1235, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(124, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_up) {
  s21_decimal val = {{1236, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(124, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_negative_down) {
  s21_decimal val = {{1234, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-123, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_negative_half_up) {
  s21_decimal val = {{1235, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-124, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_less_than_half) {
  s21_decimal val = {{4, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected = S21_DECIMAL_ZERO;

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_exactly_half) {
  s21_decimal val = {{5, 0, 0, 0}};
  s21_set_exponent(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(1, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_multi_digit_fraction) {
  s21_decimal val = {{999, 0, 0, 0}};
  s21_set_exponent(3, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(1, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_small_fraction) {
  s21_decimal val = {{1, 0, 0, 0}};
  s21_set_exponent(3, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected = S21_DECIMAL_ZERO;

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_large_number_many_decimals) {
  s21_decimal val = {{0xACD05F15, 0x01B69B4B, 0, 0}};
  s21_set_exponent(9, &val);

  s21_decimal res;
  int err = s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(123456789, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_round_null) {
  s21_decimal val;
  s21_from_int_to_decimal(10, &val);

  int err = s21_round(val, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_round_negative_exactly_half) {
  s21_decimal val = {{5, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);

  s21_decimal res;
  s21_round(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-1, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_zero_with_scale) {
  s21_decimal val = {{0, 0, 0, 0}};
  s21_set_exponent(3, &val);

  s21_decimal res;
  int err = s21_round(val, &res);

  s21_decimal expected = S21_DECIMAL_ZERO;

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal val = S21_DECIMAL_ZERO;
  s21_decimal res;
  ck_assert_int_eq(s21_round(val, &res), 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_round_negative_point_one) {
  s21_decimal val = {{1, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_round_large_fraction) {
  s21_decimal val = {{123789, 0, 0, 0}};
  s21_set_exponent(3, &val);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_uint_eq(res.bits[0], 124);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_round_negative_large) {
  s21_decimal val = {{999, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_set_sign(1, &val);
  s21_decimal res;
  s21_round(val, &res);
  ck_assert_uint_eq(res.bits[0], 100);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

Suite* suite_round(void) {
  Suite* s = suite_create("suite_round");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_round_integer);
  tcase_add_test(tc, test_round_down);
  tcase_add_test(tc, test_round_half_up);
  tcase_add_test(tc, test_round_up);
  tcase_add_test(tc, test_round_negative_down);
  tcase_add_test(tc, test_round_negative_half_up);
  tcase_add_test(tc, test_round_less_than_half);
  tcase_add_test(tc, test_round_exactly_half);
  tcase_add_test(tc, test_round_multi_digit_fraction);
  tcase_add_test(tc, test_round_small_fraction);
  tcase_add_test(tc, test_round_large_number_many_decimals);
  tcase_add_test(tc, test_round_null);
  tcase_add_test(tc, test_round_negative_exactly_half);
  tcase_add_test(tc, test_round_zero_with_scale);

  tcase_add_test(tc, test_round_zero);
  tcase_add_test(tc, test_round_negative_point_one);
  tcase_add_test(tc, test_round_large_fraction);
  tcase_add_test(tc, test_round_negative_large);

  suite_add_tcase(s, tc);
  return s;
}