#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_int) {
  s21_decimal d = {{12345, 0, 0, 0}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(res, 12345.0f, 1e-3f);
}
END_TEST

START_TEST(test_neg_float) {
  s21_decimal d = {{12345, 0, 0, (1u << 31) | (4 << 16)}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(res, -1.2345f, 1e-6f);
}
END_TEST

START_TEST(test_pos_float) {
  s21_decimal d = {{12345, 0, 0, (4 << 16)}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(res, 1.2345f, 1e-6f);
}
END_TEST

START_TEST(test_max_decimal) {
  s21_decimal d = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_ge(res, 7.92281e+28f);
  ck_assert_float_le(res, 7.92282e+28f);
}
END_TEST

START_TEST(test_neg_max_decimal) {
  s21_decimal d = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_le(res, -7.92281e+28f);
}
END_TEST

START_TEST(test_min_scale) {
  s21_decimal d = {{1, 0, 0, (28 << 16)}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_msg(fabsf(res - 1e-28f) / 1e-28f < 1e-5f,
                "got=%.10e expected=%.10e", res, 1e-28f);
}
END_TEST

START_TEST(test_zero) {
  s21_decimal d = {{0, 0, 0, 0}};
  float res = 1.0f;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(res, 0.0f);
}
END_TEST

START_TEST(test_negative_zero) {
  s21_decimal d = {{0, 0, 0, 1u << 31}};
  float res = 1.0f;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(res, 0.0f);
}
END_TEST

START_TEST(test_null_dst) {
  s21_decimal d = {{12345, 0, 0, 0}};
  int code = s21_from_decimal_to_float(d, NULL);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_small_fraction) {
  s21_decimal d = {{5, 0, 0, (1 << 16)}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(res, 0.5f, 1e-7f);
}
END_TEST

START_TEST(test_multi_segment) {
  s21_decimal d = {{0, 1, 0, 0}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq_tol(res, 4294967296.0f, 1.0f);
}
END_TEST

START_TEST(test_precision_loss) {
  s21_decimal d = {{123456789, 0, 0, 0}};
  float res = 0;
  int code = s21_from_decimal_to_float(d, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_msg(fabsf(res - 123456789.0f) / 123456789.0f < 1e-6f, "got=%.10e",
                res);
}
END_TEST

Suite* suite_from_decimal_to_float(void) {
  Suite* s = suite_create("suite_from_decimal_to_float");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_int);
  tcase_add_test(tc, test_neg_float);
  tcase_add_test(tc, test_pos_float);
  tcase_add_test(tc, test_max_decimal);
  tcase_add_test(tc, test_neg_max_decimal);
  tcase_add_test(tc, test_min_scale);
  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_negative_zero);
  tcase_add_test(tc, test_null_dst);
  tcase_add_test(tc, test_small_fraction);
  tcase_add_test(tc, test_multi_segment);
  tcase_add_test(tc, test_precision_loss);

  suite_add_tcase(s, tc);
  return s;
}