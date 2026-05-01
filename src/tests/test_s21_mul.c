#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_mul_simple) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(2, &a);
  s21_from_int_to_decimal(3, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(6, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_mul_zero_left) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(5, &b);

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_mul_zero_right) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(0, &b);

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_mul_zero_both) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(0, &b);

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_mul_by_one) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(42, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(42, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_mul_negative_left) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-2, &a);
  s21_from_int_to_decimal(3, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-6, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_mul_negative_right) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(2, &a);
  s21_from_int_to_decimal(-3, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(-6, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_mul_both_negative) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(-2, &a);
  s21_from_int_to_decimal(-3, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(6, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_mul_half_times_two) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  s21_decimal expected;
  s21_from_int_to_decimal(1, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_mul_fractional) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(1, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_set_exponent(2, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_mul_fractional_product) {
  s21_decimal a = {{15, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{25, 0, 0, 0}};
  s21_set_exponent(1, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  s21_decimal expected = {{375, 0, 0, 0}};
  s21_set_exponent(2, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_mul_negative_fractional) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_set_sign(MINUS, &a);
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_exponent(1, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  s21_decimal expected = {{25, 0, 0, 0}};
  s21_set_exponent(2, &expected);
  s21_set_sign(MINUS, &expected);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_mul_million) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(1000000, &a);
  s21_from_int_to_decimal(1000000, &b);

  int err = s21_mul(a, b, &res);

  s21_decimal expected = {{0xD4A51000, 0xE8, 0, 0}};

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_mul_int_max_squared) {
  s21_decimal a, b, res;
  s21_from_int_to_decimal(2147483647, &a);
  s21_from_int_to_decimal(2147483647, &b);

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
  s21_decimal back;
  s21_div(res, a, &back);
  ck_assert_int_eq(s21_is_equal(back, a), 1);
}
END_TEST

START_TEST(test_mul_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_mul_overflow_negative) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_set_sign(MINUS, &a);
  s21_decimal b;
  s21_from_int_to_decimal(2, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(test_mul_overflow_negative_via_sign) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b;
  s21_from_int_to_decimal(-2, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 2);
}
END_TEST

START_TEST(test_mul_max_by_tenth) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(1, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(test_mul_exp_overflow) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_set_exponent(14, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(15, &b);
  s21_decimal res;

  int err = s21_mul(a, b, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

START_TEST(test_mul_null) {
  s21_decimal a, b;
  s21_from_int_to_decimal(2, &a);
  s21_from_int_to_decimal(3, &b);

  int err = s21_mul(a, b, NULL);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_mul_commutative) {
  s21_decimal a, b, res1, res2;
  s21_from_int_to_decimal(123, &a);
  s21_from_int_to_decimal(456, &b);

  s21_mul(a, b, &res1);
  s21_mul(b, a, &res2);

  ck_assert_int_eq(s21_is_equal(res1, res2), 1);
}
END_TEST

START_TEST(test_mul_small_scale_product) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_set_exponent(7, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(7, &b);
  s21_decimal res;
  int err = s21_mul(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 1);
  ck_assert_int_eq(s21_get_exponent(&res), 14);
}
END_TEST

START_TEST(test_mul_large_scale_needs_trim) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_exponent(20, &a);
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_exponent(20, &b);
  s21_decimal res;
  int err = s21_mul(a, b, &res);
  ck_assert_int_ne(err, 3);
}
END_TEST

START_TEST(test_mul_max_by_one) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal res;
  int err = s21_mul(max, one, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[2], 0xFFFFFFFF);
}
END_TEST

START_TEST(test_mul_signs) {
  s21_decimal a = {{6, 0, 0, 0}};
  s21_decimal b = {{7, 0, 0, 0}};
  s21_decimal res;

  s21_mul(a, b, &res);
  ck_assert_int_eq(s21_get_sign(&res), 0);

  s21_set_sign(1, &b);
  s21_mul(a, b, &res);
  ck_assert_int_eq(s21_get_sign(&res), 1);

  s21_set_sign(1, &a);
  s21_mul(a, b, &res);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

Suite* suite_mul(void) {
  Suite* s = suite_create("suite_mul");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_mul_simple);
  tcase_add_test(tc, test_mul_zero_left);
  tcase_add_test(tc, test_mul_zero_right);
  tcase_add_test(tc, test_mul_zero_both);
  tcase_add_test(tc, test_mul_by_one);

  tcase_add_test(tc, test_mul_negative_left);
  tcase_add_test(tc, test_mul_negative_right);
  tcase_add_test(tc, test_mul_both_negative);

  tcase_add_test(tc, test_mul_half_times_two);
  tcase_add_test(tc, test_mul_fractional);
  tcase_add_test(tc, test_mul_fractional_product);
  tcase_add_test(tc, test_mul_negative_fractional);

  tcase_add_test(tc, test_mul_million);
  tcase_add_test(tc, test_mul_int_max_squared);

  tcase_add_test(tc, test_mul_overflow);
  tcase_add_test(tc, test_mul_overflow_negative);
  tcase_add_test(tc, test_mul_overflow_negative_via_sign);

  tcase_add_test(tc, test_mul_max_by_tenth);
  tcase_add_test(tc, test_mul_exp_overflow);

  tcase_add_test(tc, test_mul_null);
  tcase_add_test(tc, test_mul_commutative);

  tcase_add_test(tc, test_mul_small_scale_product);
  tcase_add_test(tc, test_mul_large_scale_needs_trim);
  tcase_add_test(tc, test_mul_max_by_one);
  tcase_add_test(tc, test_mul_signs);

  suite_add_tcase(s, tc);
  return s;
}