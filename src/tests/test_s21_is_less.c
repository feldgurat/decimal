#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_less_positive_simple) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_positive_false) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_equal_numbers) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_negative_numbers) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_negative_false) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_different_signs) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(1, &a);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_different_signs_reverse) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_with_scale) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);
  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_with_scale_true) {
  s21_decimal a = {{49, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_zero_and_negative_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_set_sign(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_zero_and_positive) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_negative_and_zero) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_set_sign(1, &a);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_fractional) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_set_exponent(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_fractional_false) {
  s21_decimal a = {{2, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_set_exponent(1, &b);

  int result = s21_is_less(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

Suite* suite_is_less(void) {
  Suite* s = suite_create("suite_is_less");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_less_positive_simple);
  tcase_add_test(tc, test_less_positive_false);
  tcase_add_test(tc, test_less_equal_numbers);
  tcase_add_test(tc, test_less_negative_numbers);
  tcase_add_test(tc, test_less_negative_false);
  tcase_add_test(tc, test_less_different_signs);
  tcase_add_test(tc, test_less_different_signs_reverse);
  tcase_add_test(tc, test_less_with_scale);
  tcase_add_test(tc, test_less_with_scale_true);
  tcase_add_test(tc, test_less_zero_and_negative_zero);
  tcase_add_test(tc, test_less_zero_and_positive);
  tcase_add_test(tc, test_less_negative_and_zero);
  tcase_add_test(tc, test_less_fractional);
  tcase_add_test(tc, test_less_fractional_false);

  suite_add_tcase(s, tc);
  return s;
}