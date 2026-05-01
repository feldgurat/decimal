#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_add_positive_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(&result), 0);
  ck_assert_int_eq(s21_get_exponent(&result), 0);
}
END_TEST

START_TEST(test_add_negative_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_add_both_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  s21_set_sign(1, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_add_positive_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_add_negative_positive) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_add_with_scale) {
  s21_decimal a = {{12345, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(2, &a);
  s21_set_exponent(1, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12395);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_add_with_zero) {
  s21_decimal a = {{12345, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(2, &a);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_add_fractional) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(1, &a);
  s21_set_exponent(2, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 11);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_add_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_add_overflow_with_scale) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(28, &a);
  s21_set_exponent(28, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(test_add_negative_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  s21_set_sign(1, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(test_add_zero_to_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_add_with_different_scales) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(3, &a);
  s21_set_exponent(0, &b);
  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 1001);
  ck_assert_int_eq(s21_get_exponent(&result), 3);
}
END_TEST

START_TEST(test_add_large_numbers) {
  s21_decimal a = {{1000000, 0, 0, 0}};
  s21_decimal b = {{2000000, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_add(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 3000000);
}
END_TEST

START_TEST(test_add_null_pointer) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  int err = s21_add(a, b, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_add_negative_zero) {
  s21_decimal a = S21_DECIMAL_ZERO;
  s21_set_sign(1, &a);
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal res;
  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 5);
  ck_assert_int_eq(s21_get_sign(&res), 0);
}
END_TEST

START_TEST(test_add_max_plus_zero) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal zero = S21_DECIMAL_ZERO;
  s21_decimal res;
  int err = s21_add(max, zero, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq(res.bits[2], 0xFFFFFFFF);
}
END_TEST

START_TEST(test_add_opposite_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &b);
  s21_decimal res;
  int err = s21_add(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_is_zero(&res), 1);
}
END_TEST

Suite* suite_add(void) {
  Suite* s = suite_create("suite_add");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_add_positive_simple);
  tcase_add_test(tc, test_add_negative_simple);
  tcase_add_test(tc, test_add_both_negative);
  tcase_add_test(tc, test_add_positive_negative);
  tcase_add_test(tc, test_add_negative_positive);
  tcase_add_test(tc, test_add_with_scale);
  tcase_add_test(tc, test_add_with_zero);
  tcase_add_test(tc, test_add_fractional);
  tcase_add_test(tc, test_add_overflow);
  tcase_add_test(tc, test_add_overflow_with_scale);
  tcase_add_test(tc, test_add_negative_overflow);
  tcase_add_test(tc, test_add_zero_to_zero);
  tcase_add_test(tc, test_add_with_different_scales);
  tcase_add_test(tc, test_add_large_numbers);

  tcase_add_test(tc, test_add_null_pointer);
  tcase_add_test(tc, test_add_negative_zero);
  tcase_add_test(tc, test_add_max_plus_zero);
  tcase_add_test(tc, test_add_opposite_equal);

  suite_add_tcase(s, tc);
  return s;
}