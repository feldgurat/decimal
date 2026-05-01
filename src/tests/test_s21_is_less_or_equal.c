#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_less_or_equal_less) {
  s21_decimal a = {{3, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_or_equal_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_or_equal_greater) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};

  int result = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_less_or_equal_negative_less) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_or_equal_negative_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_less_or_equal_zeros) {
  s21_decimal a = S21_DECIMAL_ZERO;
  s21_decimal b = S21_DECIMAL_ZERO;
  s21_set_sign(1, &b);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_less_or_equal(b, a), 1);
}
END_TEST

START_TEST(test_less_or_equal_different_scales) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_less_or_equal_positive_and_negative) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_less_or_equal(b, a), 0);
}
END_TEST

START_TEST(test_less_or_equal_fractional) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_less_or_equal_max) {
  s21_decimal max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal max2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(max, max2), 1);

  s21_decimal less = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(less, max), 1);
  ck_assert_int_eq(s21_is_less_or_equal(max, less), 0);
}
END_TEST

Suite* suite_is_less_or_equal(void) {
  Suite* s = suite_create("suite_is_less_or_equal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_less_or_equal_less);
  tcase_add_test(tc, test_less_or_equal_equal);
  tcase_add_test(tc, test_less_or_equal_greater);
  tcase_add_test(tc, test_less_or_equal_negative_less);
  tcase_add_test(tc, test_less_or_equal_negative_equal);

  tcase_add_test(tc, test_less_or_equal_zeros);
  tcase_add_test(tc, test_less_or_equal_different_scales);
  tcase_add_test(tc, test_less_or_equal_positive_and_negative);
  tcase_add_test(tc, test_less_or_equal_fractional);
  tcase_add_test(tc, test_less_or_equal_max);

  suite_add_tcase(s, tc);
  return s;
}