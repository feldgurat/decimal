#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_equal_positive_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_positive_different) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_negative_simple) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_negative_different) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_set_sign(1, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_different_signs) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_equal_different_scales) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_exponent(1, &a);  // 5.0

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_negative_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_set_sign(1, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_fractional) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_set_exponent(1, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_fractional_different_scales) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_set_exponent(2, &b);

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_equal_max_decimal) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

  int result = s21_is_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

Suite* suite_is_equal(void) {
  Suite* s = suite_create("suite_is_equal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_equal_positive_simple);
  tcase_add_test(tc, test_equal_positive_different);
  tcase_add_test(tc, test_equal_negative_simple);
  tcase_add_test(tc, test_equal_negative_different);
  tcase_add_test(tc, test_equal_different_signs);
  tcase_add_test(tc, test_equal_different_scales);
  tcase_add_test(tc, test_equal_zero);
  tcase_add_test(tc, test_equal_negative_zero);
  tcase_add_test(tc, test_equal_fractional);
  tcase_add_test(tc, test_equal_fractional_different_scales);
  tcase_add_test(tc, test_equal_max_decimal);

  suite_add_tcase(s, tc);
  return s;
}