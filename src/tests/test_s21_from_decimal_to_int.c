#include <check.h>
#include <limits.h>

#include "../s21_common.h"
#include "../s21_decimal.h"

START_TEST(test_from_decimal_to_int_normal) {
  s21_decimal src = {{123, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 123);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative) {
  s21_decimal src = {{123, 0, 0, 0}};
  s21_set_sign(MINUS, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, -123);
}
END_TEST

START_TEST(test_from_decimal_to_int_with_fraction) {
  s21_decimal src = {{12345, 0, 0, 0}};
  s21_set_exponent(2, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 123);
}
END_TEST

START_TEST(test_from_decimal_to_int_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_set_sign(MINUS, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_point_nine) {
  s21_decimal src = {{9, 0, 0, 0}};
  s21_set_exponent(1, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_max) {
  s21_decimal src = {{2147483647, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 2147483647);
}
END_TEST

START_TEST(test_from_decimal_to_int_max_plus_one) {
  s21_decimal src = {{2147483648u, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_min) {
  s21_decimal src = {{2147483648u, 0, 0, 0}};
  s21_set_sign(MINUS, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, INT_MIN);
}
END_TEST

START_TEST(test_from_decimal_to_int_min_minus_one) {
  s21_decimal src = {{2147483649u, 0, 0, 0}};
  s21_set_sign(MINUS, &src);
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_large) {
  s21_decimal src = {{0, 0, 1, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_null_pointer) {
  s21_decimal src = {{123, 0, 0, 0}};
  int error = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(error, 1);
}
END_TEST

Suite* suite_from_decimal_to_int(void) {
  Suite* s = suite_create("suite_from_decimal_to_int");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_from_decimal_to_int_normal);
  tcase_add_test(tc, test_from_decimal_to_int_negative);
  tcase_add_test(tc, test_from_decimal_to_int_with_fraction);
  tcase_add_test(tc, test_from_decimal_to_int_zero);
  tcase_add_test(tc, test_from_decimal_to_int_negative_zero);
  tcase_add_test(tc, test_from_decimal_to_int_point_nine);
  tcase_add_test(tc, test_from_decimal_to_int_max);
  tcase_add_test(tc, test_from_decimal_to_int_max_plus_one);
  tcase_add_test(tc, test_from_decimal_to_int_min);
  tcase_add_test(tc, test_from_decimal_to_int_min_minus_one);
  tcase_add_test(tc, test_from_decimal_to_int_large);
  tcase_add_test(tc, test_from_decimal_to_int_null_pointer);

  suite_add_tcase(s, tc);
  return s;
}