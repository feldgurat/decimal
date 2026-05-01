#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_negate_positive) {
  s21_decimal val, res;
  s21_from_int_to_decimal(123, &val);

  int err = s21_negate(val, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal val, res;
  s21_from_int_to_decimal(-456, &val);

  int err = s21_negate(val, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_mantiss_dont_change) {
  s21_decimal val, res;
  s21_from_int_to_decimal(789, &val);

  s21_negate(val, &res);

  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal val = S21_DECIMAL_ZERO;
  s21_decimal res;

  int err = s21_negate(val, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_compare_mantiss(&val, &res), 0);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_negate_null) {
  s21_decimal val;
  s21_from_int_to_decimal(10, &val);

  int err = s21_negate(val, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_negate_double) {
  s21_decimal val = {{42, 0, 0, 0}};
  s21_set_exponent(5, &val);
  s21_decimal tmp, res;
  s21_negate(val, &tmp);
  s21_negate(tmp, &res);
  ck_assert_uint_eq(res.bits[0], val.bits[0]);
  ck_assert_uint_eq(res.bits[3], val.bits[3]);
}
END_TEST

START_TEST(test_negate_preserves_scale) {
  s21_decimal val = {{999, 0, 0, 0}};
  s21_set_exponent(15, &val);
  s21_decimal res;
  s21_negate(val, &res);
  ck_assert_int_eq(s21_get_exponent(&res), 15);
  ck_assert_uint_eq(res.bits[0], 999);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_negate_max) {
  s21_decimal val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal res;
  s21_negate(val, &res);
  ck_assert_uint_eq(res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

Suite* suite_negate(void) {
  Suite* s = suite_create("suite_negate");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_negate_positive);
  tcase_add_test(tc, test_negate_negative);
  tcase_add_test(tc, test_mantiss_dont_change);
  tcase_add_test(tc, test_negate_zero);
  tcase_add_test(tc, test_negate_null);

  tcase_add_test(tc, test_negate_double);
  tcase_add_test(tc, test_negate_preserves_scale);
  tcase_add_test(tc, test_negate_max);

  suite_add_tcase(s, tc);
  return s;
}