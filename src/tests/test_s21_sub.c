#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_sub_positive_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(&result), 0);
  ck_assert_int_eq(s21_get_exponent(&result), 0);
}
END_TEST

START_TEST(test_sub_negative_result) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_sign(&result), 1);
  ck_assert_int_eq(s21_get_exponent(&result), 0);
}
END_TEST

START_TEST(test_sub_negative_numbers) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  s21_set_sign(1, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_sub_negative_minus_positive) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_sub_positive_minus_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 8);
  ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_sub_with_scale) {
  s21_decimal a = {{12345, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(2, &a);
  s21_set_exponent(1, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12295);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_sub_with_scale_negative_result) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{12345, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(1, &a);
  s21_set_exponent(2, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12295);
  ck_assert_int_eq(s21_get_sign(&result), 1);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_sub_with_zero) {
  s21_decimal a = {{12345, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(2, &a);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_sub_zero_minus_number) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{12345, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(2, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(s21_get_sign(&result), 1);
  ck_assert_int_eq(s21_get_exponent(&result), 2);
}
END_TEST

START_TEST(test_sub_fractional) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(1, &a);
  s21_set_exponent(1, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(s21_get_exponent(&result), 1);
}
END_TEST

START_TEST(test_sub_equal_numbers) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_sub_equal_numbers_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  s21_set_sign(1, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(s21_get_sign(&result), 0);
}
END_TEST

START_TEST(test_sub_with_different_scales) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(3, &a);
  s21_set_exponent(0, &b);
  int error = s21_sub(b, a, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 999);
  ck_assert_int_eq(s21_get_exponent(&result), 3);
}
END_TEST

START_TEST(test_sub_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_sign(1, &a);
  s21_set_sign(0, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(test_sub_large_numbers) {
  s21_decimal a = {{3000000, 0, 0, 0}};
  s21_decimal b = {{1000000, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2000000);
}
END_TEST

START_TEST(test_sub_large_numbers_negative) {
  s21_decimal a = {{1000000, 0, 0, 0}};
  s21_decimal b = {{3000000, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 2000000);
  ck_assert_int_eq(s21_get_sign(&result), 1);
}
END_TEST

START_TEST(test_sub_very_small) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(28, &a);
  s21_set_exponent(28, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_sub_rounding) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  s21_set_exponent(28, &a);
  s21_set_exponent(28, &b);
  int error = s21_sub(a, b, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_sub_null_pointer) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  int err = s21_sub(a, b, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_sub_from_zero) {
  s21_decimal a = S21_DECIMAL_ZERO;
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal res;
  int err = s21_sub(a, b, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_uint_eq(res.bits[0], 5);
  ck_assert_int_eq(s21_get_sign(&res), 1);
}
END_TEST

START_TEST(test_sub_negative_overflow) {
  s21_decimal neg_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_set_sign(1, &neg_max);
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal res;
  int err = s21_sub(neg_max, one, &res);
  ck_assert_int_eq(err, 2);
}
END_TEST

Suite* suite_sub(void) {
  Suite* s = suite_create("suite_sub");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_sub_positive_simple);
  tcase_add_test(tc, test_sub_negative_result);
  tcase_add_test(tc, test_sub_negative_numbers);
  tcase_add_test(tc, test_sub_negative_minus_positive);
  tcase_add_test(tc, test_sub_positive_minus_negative);

  tcase_add_test(tc, test_sub_with_scale);
  tcase_add_test(tc, test_sub_with_scale_negative_result);
  tcase_add_test(tc, test_sub_with_different_scales);

  tcase_add_test(tc, test_sub_with_zero);
  tcase_add_test(tc, test_sub_zero_minus_number);

  tcase_add_test(tc, test_sub_fractional);

  tcase_add_test(tc, test_sub_equal_numbers);
  tcase_add_test(tc, test_sub_equal_numbers_negative);

  tcase_add_test(tc, test_sub_large_numbers);
  tcase_add_test(tc, test_sub_large_numbers_negative);

  tcase_add_test(tc, test_sub_overflow);
  tcase_add_test(tc, test_sub_very_small);
  tcase_add_test(tc, test_sub_rounding);

  tcase_add_test(tc, test_sub_null_pointer);
  tcase_add_test(tc, test_sub_from_zero);
  tcase_add_test(tc, test_sub_negative_overflow);

  suite_add_tcase(s, tc);
  return s;
}