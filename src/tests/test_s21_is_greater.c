#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_greater_positive_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 1);
  ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_greater_positive_equal) {
  s21_decimal a = {{42, 0, 0, 0}};
  s21_decimal b = {{42, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_greater_negative_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_greater(b, a), 1);
}
END_TEST

START_TEST(test_greater_different_signs) {
  s21_decimal positive = {{10, 0, 0, 0}};
  s21_decimal negative = {{5, 0, 0, 0}};
  s21_set_sign(MINUS, &negative);

  ck_assert_int_eq(s21_is_greater(positive, negative), 1);
  ck_assert_int_eq(s21_is_greater(negative, positive), 0);
}
END_TEST

START_TEST(test_greater_with_exponent) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(2, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_greater_negative_with_exponent) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  s21_set_sign(MINUS, &b);
  s21_set_exponent(2, &b);
  ck_assert_int_eq(s21_is_greater(b, a), 1);
}
END_TEST

START_TEST(test_greater_large_numbers) {
  s21_decimal a = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_greater_boundary_exponent) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1000, 0, 0, 0}};
  s21_set_exponent(28, &a);
  ck_assert_int_eq(s21_is_greater(b, a), 1);
}
END_TEST

START_TEST(test_plus_and_minus_zeros) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_set_sign(MINUS, &a);
  ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_same_numbers) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  s21_set_exponent(1, &b);
  ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

Suite* suite_is_greater(void) {
  Suite* s = suite_create("suite_is_greater");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_greater_positive_simple);
  tcase_add_test(tc, test_greater_positive_equal);
  tcase_add_test(tc, test_greater_negative_simple);
  tcase_add_test(tc, test_greater_different_signs);
  tcase_add_test(tc, test_greater_with_exponent);
  tcase_add_test(tc, test_greater_negative_with_exponent);
  tcase_add_test(tc, test_greater_large_numbers);
  tcase_add_test(tc, test_greater_boundary_exponent);
  tcase_add_test(tc, test_plus_and_minus_zeros);
  tcase_add_test(tc, test_same_numbers);

  suite_add_tcase(s, tc);
  return s;
}