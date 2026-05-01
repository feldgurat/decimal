#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_div_simple) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(10, &val1);
  s21_from_int_to_decimal(2, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(5, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_div_tens) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(100, &val1);
  s21_from_int_to_decimal(10, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(10, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_by_one) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(42, &val1);
  s21_from_int_to_decimal(1, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(42, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_result_tenth) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(1, &val1);
  s21_from_int_to_decimal(10, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_set_exponent(1, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_exact_half) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(5, &val1);
  s21_from_int_to_decimal(2, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected = {{25, 0, 0, 0}};
  s21_set_exponent(1, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_quarter) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(1, &val1);
  s21_from_int_to_decimal(4, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected = {{25, 0, 0, 0}};
  s21_set_exponent(2, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_one_third) {
  s21_decimal one, three, res, back;
  s21_from_int_to_decimal(1, &one);
  s21_from_int_to_decimal(3, &three);

  int err = s21_div(one, three, &res);
  ck_assert_int_eq(err, 0);

  s21_decimal low = {{33, 0, 0, 0}};
  s21_set_exponent(2, &low);
  s21_decimal high = {{34, 0, 0, 0}};
  s21_set_exponent(2, &high);

  ck_assert_int_eq(s21_is_greater(res, low), 1);
  ck_assert_int_eq(s21_is_less(res, high), 1);

  s21_mul(res, three, &back);
  s21_decimal diff;
  s21_sub(one, back, &diff);
  s21_set_sign(PLUS, &diff);
  s21_decimal eps = {{1, 0, 0, 0}};
  s21_set_exponent(25, &eps);
  ck_assert_int_eq(s21_is_less(diff, eps), 1);
}
END_TEST

START_TEST(test_div_two_thirds) {
  s21_decimal two, three, res, back;
  s21_from_int_to_decimal(2, &two);
  s21_from_int_to_decimal(3, &three);

  int err = s21_div(two, three, &res);
  ck_assert_int_eq(err, 0);

  s21_mul(res, three, &back);
  s21_decimal diff;
  s21_sub(two, back, &diff);
  s21_set_sign(PLUS, &diff);
  s21_decimal eps = {{1, 0, 0, 0}};
  s21_set_exponent(25, &eps);
  ck_assert_int_eq(s21_is_less(diff, eps), 1);
}
END_TEST

START_TEST(test_div_negative_dividend) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(-10, &val1);
  s21_from_int_to_decimal(2, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-5, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_negative_divisor) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(10, &val1);
  s21_from_int_to_decimal(-2, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-5, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_both_negative) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(-10, &val1);
  s21_from_int_to_decimal(-2, &val2);

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(5, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_div_negative_result_exp) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(100, &val1);
  s21_decimal v2 = {{1, 0, 0, 0}};
  s21_set_exponent(2, &v2);
  val2 = v2;

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(10000, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_fractional_dividend) {
  s21_decimal val1 = {{5, 0, 0, 0}};
  s21_set_exponent(1, &val1);
  s21_decimal val2;
  s21_from_int_to_decimal(2, &val2);
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  s21_decimal expected = {{25, 0, 0, 0}};
  s21_set_exponent(2, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_both_fractional) {
  s21_decimal val1 = {{1, 0, 0, 0}};
  s21_set_exponent(1, &val1);
  s21_decimal val2 = {{1, 0, 0, 0}};
  s21_set_exponent(2, &val2);
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(10, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_by_zero) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(10, &val1);
  s21_from_int_to_decimal(0, &val2);

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 3);
}
END_TEST

START_TEST(test_div_negative_by_zero) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(-10, &val1);
  s21_from_int_to_decimal(0, &val2);

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 3);
}
END_TEST

START_TEST(test_div_zero_by_zero) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(0, &val1);
  s21_from_int_to_decimal(0, &val2);

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 3);
}
END_TEST

START_TEST(test_div_zero_dividend) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(0, &val1);
  s21_from_int_to_decimal(5, &val2);

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_div_zero_by_negative) {
  s21_decimal val1, val2, res;
  s21_from_int_to_decimal(0, &val1);
  s21_from_int_to_decimal(-5, &val2);

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_div_max_by_one) {
  s21_decimal val1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal val2;
  s21_from_int_to_decimal(1, &val2);
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, val1), 1);
}
END_TEST

START_TEST(test_div_max_by_max) {
  s21_decimal val1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal val2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(1, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_div_overflow) {
  s21_decimal val1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal val2 = {{1, 0, 0, 0}};
  s21_set_exponent(1, &val2);
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_div_overflow_negative) {
  s21_decimal val1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_set_sign(MINUS, &val1);
  s21_decimal val2 = {{1, 0, 0, 0}};
  s21_set_exponent(1, &val2);
  s21_decimal res;

  int err = s21_div(val1, val2, &res);

  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(test_div_null_result) {
  s21_decimal val1, val2;
  s21_from_int_to_decimal(10, &val1);
  s21_from_int_to_decimal(2, &val2);

  int err = s21_div(val1, val2, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_div_roundtrip) {
  int pairs[][2] = {{100, 7}, {1000, 13}, {50, 4}, {99, 11}, {12345, 67}};
  int n = sizeof(pairs) / sizeof(pairs[0]);

  for (int i = 0; i < n; i++) {
    s21_decimal a, b, q, back, diff, eps;
    s21_from_int_to_decimal(pairs[i][0], &a);
    s21_from_int_to_decimal(pairs[i][1], &b);

    int err = s21_div(a, b, &q);
    ck_assert_int_eq(err, 0);

    s21_mul(q, b, &back);
    s21_sub(a, back, &diff);
    s21_set_sign(PLUS, &diff);

    s21_from_int_to_decimal(1, &eps);
    s21_set_exponent(20, &eps);

    ck_assert_msg(s21_is_less(diff, eps), "pair %d/%d failed", pairs[i][0],
                  pairs[i][1]);
  }
}
END_TEST

START_TEST(test_div_small_by_large) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal res;
  int err = s21_div(one, max, &res);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(test_div_negative_dividend_positive_divisor) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_decimal b = {{4, 0, 0, 0}};
  s21_decimal res;
  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 25);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_div_one_by_one) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res;
  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  float f;
  s21_from_decimal_to_float(res, &f);
  ck_assert_float_eq(f, 1.0f);
}
END_TEST

START_TEST(test_div_with_scale) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{20, 0, 0, 0}};
  s21_set_exponent(1, &b);
  s21_decimal res;
  int err = s21_div(a, b, &res);
  ck_assert_int_eq(err, 0);
  float f;
  s21_from_decimal_to_float(res, &f);
  ck_assert_float_eq_tol(f, 0.5f, 1e-7);
}
END_TEST

START_TEST(test_div_zero_by_number) {
  s21_decimal zero = S21_DECIMAL_ZERO;
  s21_decimal b = {{42, 0, 0, 0}};
  s21_decimal res;
  int err = s21_div(zero, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

Suite* suite_div(void) {
  Suite* s = suite_create("suite_div");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_div_simple);
  tcase_add_test(tc, test_div_tens);
  tcase_add_test(tc, test_div_by_one);

  tcase_add_test(tc, test_div_result_tenth);
  tcase_add_test(tc, test_div_exact_half);
  tcase_add_test(tc, test_div_quarter);
  tcase_add_test(tc, test_div_one_third);
  tcase_add_test(tc, test_div_two_thirds);

  tcase_add_test(tc, test_div_negative_dividend);
  tcase_add_test(tc, test_div_negative_divisor);
  tcase_add_test(tc, test_div_both_negative);

  tcase_add_test(tc, test_div_negative_result_exp);
  tcase_add_test(tc, test_div_fractional_dividend);
  tcase_add_test(tc, test_div_both_fractional);

  tcase_add_test(tc, test_div_by_zero);
  tcase_add_test(tc, test_div_negative_by_zero);
  tcase_add_test(tc, test_div_zero_by_zero);
  tcase_add_test(tc, test_div_zero_dividend);
  tcase_add_test(tc, test_div_zero_by_negative);

  tcase_add_test(tc, test_div_max_by_one);
  tcase_add_test(tc, test_div_max_by_max);
  tcase_add_test(tc, test_div_overflow);
  tcase_add_test(tc, test_div_overflow_negative);

  tcase_add_test(tc, test_div_null_result);
  tcase_add_test(tc, test_div_roundtrip);

  tcase_add_test(tc, test_div_small_by_large);
  tcase_add_test(tc, test_div_negative_dividend_positive_divisor);
  tcase_add_test(tc, test_div_one_by_one);
  tcase_add_test(tc, test_div_with_scale);
  tcase_add_test(tc, test_div_zero_by_number);

  suite_add_tcase(s, tc);
  return s;
}