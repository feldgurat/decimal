#include <check.h>
#include <stdlib.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_not_equal_different) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_not_equal_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_not_equal_zero_and_negative_zero) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_set_sign(1, &b);

  int result = s21_is_not_equal(a, b);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_not_equal_different_signs) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(1, &b);
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_not_equal_different_scales) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_set_exponent(1, &a);
  s21_decimal b = {{100, 0, 0, 0}};
  s21_set_exponent(2, &b);
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_not_equal_max_and_max_minus_one) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_not_equal_negative_same) {
  s21_decimal a = {{42, 0, 0, 0}};
  s21_set_sign(1, &a);
  s21_decimal b = {{42, 0, 0, 0}};
  s21_set_sign(1, &b);
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_not_equal_one_and_one_tenth) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_set_exponent(1, &b);
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

Suite* suite_is_not_equal(void) {
  Suite* s = suite_create("suite_is_not_equal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_not_equal_different);
  tcase_add_test(tc, test_not_equal_equal);
  tcase_add_test(tc, test_not_equal_zero_and_negative_zero);

  tcase_add_test(tc, test_not_equal_different_signs);
  tcase_add_test(tc, test_not_equal_different_scales);
  tcase_add_test(tc, test_not_equal_max_and_max_minus_one);
  tcase_add_test(tc, test_not_equal_negative_same);
  tcase_add_test(tc, test_not_equal_one_and_one_tenth);

  suite_add_tcase(s, tc);
  return s;
}