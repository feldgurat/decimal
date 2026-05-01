#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_truncate_integer) {
  s21_decimal val, res;
  s21_from_int_to_decimal(123, &val);

  int err = s21_truncate(val, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_truncate_positive_fraction) {
  s21_decimal val = {{123456, 0, 0, 0}};
  s21_set_exponent(3, &val);

  s21_decimal res;

  s21_truncate(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(123, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_truncate_negative_fraction) {
  s21_decimal val = {{123456, 0, 0, 0}};
  s21_set_exponent(3, &val);
  s21_set_sign(1, &val);

  s21_decimal res;

  s21_truncate(val, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-123, &expected);

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_truncate_less_than_one) {
  s21_decimal val = {{999, 0, 0, 0}};
  s21_set_exponent(3, &val);

  s21_decimal res;

  s21_truncate(val, &res);

  s21_decimal expected = S21_DECIMAL_ZERO;

  ck_assert_int_eq(s21_compare_mantiss(&res, &expected), 0);
}
END_TEST

START_TEST(test_truncate_negative_less_than_one) {
  s21_decimal val = {{999, 0, 0, 0}};
  s21_set_exponent(3, &val);
  s21_set_sign(1, &val);

  s21_decimal res;

  s21_truncate(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&res, &(s21_decimal){{0}}), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_truncate_large_scale) {
  s21_decimal val = {{1, 0, 0, 0}};
  s21_set_exponent(5, &val);

  s21_decimal res;

  s21_truncate(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&res, &(s21_decimal){{0}}), 0);
}
END_TEST

START_TEST(test_truncate_null) {
  s21_decimal val;
  s21_from_int_to_decimal(10, &val);

  int err = s21_truncate(val, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal val = S21_DECIMAL_ZERO;
  s21_decimal res;
  ck_assert_int_eq(s21_truncate(val, &res), 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_truncate_negative_zero) {
  s21_decimal val = S21_DECIMAL_ZERO;
  s21_set_sign(1, &val);
  s21_decimal res;
  ck_assert_int_eq(s21_truncate(val, &res), 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_truncate_max_no_scale) {
  s21_decimal val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal res;
  ck_assert_int_eq(s21_truncate(val, &res), 0);
  ck_assert_uint_eq(res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

START_TEST(test_truncate_point_nine) {
  s21_decimal val = {{9, 0, 0, 0}};
  s21_set_exponent(1, &val);
  s21_decimal res;
  ck_assert_int_eq(s21_truncate(val, &res), 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_truncate_preserves_sign) {
  s21_decimal val = {{123456, 0, 0, 0}};
  s21_set_exponent(3, &val);
  s21_set_sign(1, &val);
  s21_decimal res;
  ck_assert_int_eq(s21_truncate(val, &res), 0);
  ck_assert_uint_eq(res.bits[0], 123);
  ck_assert_int_eq(s21_get_sign(&res), 1);
  ck_assert_int_eq(s21_get_exponent(&res), 0);
}
END_TEST

Suite* suite_truncate(void) {
  Suite* s = suite_create("suite_truncate");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_truncate_integer);
  tcase_add_test(tc, test_truncate_positive_fraction);
  tcase_add_test(tc, test_truncate_negative_fraction);
  tcase_add_test(tc, test_truncate_less_than_one);
  tcase_add_test(tc, test_truncate_negative_less_than_one);
  tcase_add_test(tc, test_truncate_large_scale);
  tcase_add_test(tc, test_truncate_null);

  tcase_add_test(tc, test_truncate_zero);
  tcase_add_test(tc, test_truncate_negative_zero);
  tcase_add_test(tc, test_truncate_max_no_scale);
  tcase_add_test(tc, test_truncate_point_nine);
  tcase_add_test(tc, test_truncate_preserves_sign);

  suite_add_tcase(s, tc);
  return s;
}