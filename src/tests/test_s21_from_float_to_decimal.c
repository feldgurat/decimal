#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

static double decimal_to_double(s21_decimal d) {
  double result = 0;
  double base = 1;
  for (int i = 0; i < 3; i++) {
    result += (double)d.bits[i] * base;
    base *= 4294967296.0;
  }
  int exp = s21_get_exponent(&d);
  for (int i = 0; i < exp; i++) result /= 10.0;
  if (s21_get_sign(&d)) result = -result;
  return result;
}

static void assert_float_eq(float src, s21_decimal d) {
  double got = decimal_to_double(d);
  if (src == 0.0f) {
    ck_assert_double_eq(got, 0.0);
    return;
  }
  double rel_err = fabs((got - (double)src) / (double)src);
  ck_assert_msg(rel_err < 1e-5, "src=%.8e got=%.8e rel_err=%.8e", src, got,
                rel_err);
}

START_TEST(test_zero) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.0f, &d);
  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(d.bits[0], 0);
  ck_assert_uint_eq(d.bits[1], 0);
  ck_assert_uint_eq(d.bits[2], 0);
}
END_TEST

START_TEST(test_negative_zero) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-0.0f, &d);
  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(d.bits[0], 0);
  ck_assert_uint_eq(d.bits[1], 0);
  ck_assert_uint_eq(d.bits[2], 0);
}
END_TEST

START_TEST(test_positive_inf) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(INFINITY, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_negative_inf) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-INFINITY, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_nan) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(NAN, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_too_small_positive) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1e-29f, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_too_small_negative) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-1e-29f, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_one) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(1.0f, d);
  ck_assert_int_eq(s21_get_sign(&d), 0);
}
END_TEST

START_TEST(test_negative_one) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-1.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(-1.0f, d);
  ck_assert_int_eq(s21_get_sign(&d), 1);
}
END_TEST

START_TEST(test_two) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(2.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(2.0f, d);
}
END_TEST

START_TEST(test_ten) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(10.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(10.0f, d);
}
END_TEST

START_TEST(test_hundred) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(100.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(100.0f, d);
}
END_TEST

START_TEST(test_million) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1000000.0f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(1000000.0f, d);
}
END_TEST

START_TEST(test_large_int) {
  s21_decimal d;
  float val = 1234567.0f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
}
END_TEST

START_TEST(test_half) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.5f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(0.5f, d);
}
END_TEST

START_TEST(test_quarter) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.25f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(0.25f, d);
}
END_TEST

START_TEST(test_one_tenth) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.1f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(0.1f, d);
}
END_TEST

START_TEST(test_one_hundredth) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.01f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(0.01f, d);
}
END_TEST

START_TEST(test_one_eighth) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(0.125f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(0.125f, d);
}
END_TEST

START_TEST(test_pi) {
  s21_decimal d;
  float val = 3.14159f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
}
END_TEST

START_TEST(test_neg_half) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-0.5f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(-0.5f, d);
  ck_assert_int_eq(s21_get_sign(&d), 1);
}
END_TEST

START_TEST(test_neg_pi) {
  s21_decimal d;
  float val = -3.14159f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
  ck_assert_int_eq(s21_get_sign(&d), 1);
}
END_TEST

START_TEST(test_small_boundary) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1e-28f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(1e-28f, d);
}
END_TEST

START_TEST(test_large_float) {
  s21_decimal d;
  float val = 1e+20f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
}
END_TEST

START_TEST(test_max_safe_float) {
  s21_decimal d;
  float val = 7.9e+28f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
}
END_TEST

START_TEST(test_seven_digits) {
  s21_decimal d;
  float val = 1.234567f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
  ck_assert_msg(s21_count_digits(&d) <= 7, "too many significant digits");
}
END_TEST

START_TEST(test_digits_count_small) {
  s21_decimal d;
  float val = 0.001234f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
  ck_assert_msg(s21_count_digits(&d) <= 7, "too many significant digits");
}
END_TEST

START_TEST(test_exact_power_of_two) {
  s21_decimal d;
  float val = 1024.0f;
  int res = s21_from_float_to_decimal(val, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(val, d);
}
END_TEST

START_TEST(test_sign_preserved) {
  s21_decimal d;
  const float vals[] = {1.0f, -1.0f, 42.5f, -42.5f, 0.001f, -0.001f};
  int signs[] = {0, 1, 0, 1, 0, 1};
  for (int i = 0; i < 6; i++) {
    int res = s21_from_float_to_decimal(vals[i], &d);
    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(s21_get_sign(&d), signs[i]);
  }
}
END_TEST

START_TEST(test_float_null_pointer) {
  int res = s21_from_float_to_decimal(1.0f, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_float_very_large) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1e+38f, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_float_negative_very_large) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-1e+38f, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_float_1e_27) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1e-27f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(1e-27f, d);
}
END_TEST

START_TEST(test_float_negative_small_boundary) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(-1e-28f, &d);
  ck_assert_int_eq(res, 0);
  assert_float_eq(-1e-28f, d);
}
END_TEST

START_TEST(test_float_denormalized) {
  s21_decimal d;
  int res = s21_from_float_to_decimal(1e-40f, &d);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_float_powers_of_ten) {
  const float vals[] = {1e-20f, 1e-10f, 1e-5f, 1e0f, 1e5f, 1e10f, 1e20f};
  for (int i = 0; i < 7; i++) {
    s21_decimal d;
    int res = s21_from_float_to_decimal(vals[i], &d);
    ck_assert_int_eq(res, 0);
    assert_float_eq(vals[i], d);
  }
}
END_TEST

Suite* suite_from_float_to_decimal(void) {
  Suite* s = suite_create("suite_from_float_to_decimal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_negative_zero);

  tcase_add_test(tc, test_positive_inf);
  tcase_add_test(tc, test_negative_inf);
  tcase_add_test(tc, test_nan);
  tcase_add_test(tc, test_too_small_positive);
  tcase_add_test(tc, test_too_small_negative);

  tcase_add_test(tc, test_one);
  tcase_add_test(tc, test_negative_one);
  tcase_add_test(tc, test_two);
  tcase_add_test(tc, test_ten);
  tcase_add_test(tc, test_hundred);
  tcase_add_test(tc, test_million);
  tcase_add_test(tc, test_large_int);

  tcase_add_test(tc, test_half);
  tcase_add_test(tc, test_quarter);
  tcase_add_test(tc, test_one_tenth);
  tcase_add_test(tc, test_one_hundredth);
  tcase_add_test(tc, test_one_eighth);
  tcase_add_test(tc, test_pi);

  tcase_add_test(tc, test_neg_half);
  tcase_add_test(tc, test_neg_pi);

  tcase_add_test(tc, test_small_boundary);
  tcase_add_test(tc, test_large_float);
  tcase_add_test(tc, test_max_safe_float);

  tcase_add_test(tc, test_seven_digits);
  tcase_add_test(tc, test_digits_count_small);
  tcase_add_test(tc, test_exact_power_of_two);
  tcase_add_test(tc, test_sign_preserved);

  tcase_add_test(tc, test_float_null_pointer);
  tcase_add_test(tc, test_float_very_large);
  tcase_add_test(tc, test_float_negative_very_large);
  tcase_add_test(tc, test_float_1e_27);
  tcase_add_test(tc, test_float_negative_small_boundary);
  tcase_add_test(tc, test_float_denormalized);
  tcase_add_test(tc, test_float_powers_of_ten);

  suite_add_tcase(s, tc);
  return s;
}