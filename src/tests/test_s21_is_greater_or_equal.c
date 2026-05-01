#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_greater_or_equal_positive_greater) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_positive_equal) {
  s21_decimal a = {{42, 0, 0, 0}};
  s21_decimal b = {{42, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_positive_less) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{7, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_greater_or_equal_negative_greater) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_negative_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_negative_less) {
  s21_decimal a = {{7, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_greater_or_equal_different_signs_positive_greater) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_different_signs_negative_greater) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_decimal b = {{10, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_greater_or_equal_with_exponent_greater) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(2, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_with_exponent_equal) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_set_exponent(2, &a);
  s21_set_exponent(4, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_with_exponent_less) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_set_exponent(2, &a);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_greater_or_equal_negative_with_exponent) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  s21_set_exponent(2, &a);
  s21_set_exponent(0, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_zero_and_negative) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_large_numbers) {
  s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal b = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

Suite* suite_is_greater_or_equal(void) {
  Suite* s = suite_create("suite_is_greater_or_equal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_greater_or_equal_positive_greater);
  tcase_add_test(tc, test_greater_or_equal_positive_equal);
  tcase_add_test(tc, test_greater_or_equal_positive_less);
  tcase_add_test(tc, test_greater_or_equal_negative_greater);
  tcase_add_test(tc, test_greater_or_equal_negative_equal);
  tcase_add_test(tc, test_greater_or_equal_negative_less);
  tcase_add_test(tc, test_greater_or_equal_different_signs_positive_greater);
  tcase_add_test(tc, test_greater_or_equal_different_signs_negative_greater);
  tcase_add_test(tc, test_greater_or_equal_with_exponent_greater);
  tcase_add_test(tc, test_greater_or_equal_with_exponent_equal);
  tcase_add_test(tc, test_greater_or_equal_with_exponent_less);
  tcase_add_test(tc, test_greater_or_equal_negative_with_exponent);
  tcase_add_test(tc, test_greater_or_equal_zero);
  tcase_add_test(tc, test_greater_or_equal_zero_and_negative);
  tcase_add_test(tc, test_greater_or_equal_large_numbers);

  suite_add_tcase(s, tc);
  return s;
}