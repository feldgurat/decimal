#include <check.h>
#include <stdlib.h>

#include "../s21_decimal.h"

START_TEST(test_positive_ints) {
  for (long long i = 0; i < INT32_MAX; i *= 2) {
    s21_decimal dec = S21_DECIMAL_ZERO;
    int res = s21_from_int_to_decimal((int)i, &dec);
    ck_assert_int_eq(res, 0);
    ck_assert_uint_eq(dec.bits[0], (uint32_t)i);
    ck_assert_uint_eq(dec.bits[1], 0);
    ck_assert_uint_eq(dec.bits[2], 0);
    ck_assert_uint_eq(dec.bits[3], 0);
    if (i == 0) i++;
  }
}
END_TEST

START_TEST(test_negative_ints) {
  for (long long i = -1; i > INT32_MIN; i *= 2) {
    s21_decimal dec = S21_DECIMAL_ZERO;
    int res = s21_from_int_to_decimal((int)i, &dec);
    ck_assert_int_eq(res, 0);
    ck_assert_uint_eq(dec.bits[0], (uint32_t)(-i));
    ck_assert_uint_eq(dec.bits[1], 0);
    ck_assert_uint_eq(dec.bits[2], 0);
    ck_assert_uint_eq(dec.bits[3], 0x80000000);
  }
}
END_TEST

START_TEST(test_int_max) {
  s21_decimal dec = S21_DECIMAL_ZERO;
  int res = s21_from_int_to_decimal(INT32_MAX, &dec);
  ck_assert_uint_eq(dec.bits[0], (uint32_t)(INT32_MAX));
  ck_assert_uint_eq(dec.bits[1], 0);
  ck_assert_uint_eq(dec.bits[2], 0);
  ck_assert_uint_eq(dec.bits[3], 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(test_int_min) {
  s21_decimal dec = S21_DECIMAL_ZERO;
  int res = s21_from_int_to_decimal(INT32_MIN, &dec);
  ck_assert_uint_eq(dec.bits[0], (uint32_t)(-(long long)INT32_MIN));
  ck_assert_uint_eq(dec.bits[1], 0);
  ck_assert_uint_eq(dec.bits[2], 0);
  ck_assert_uint_eq(dec.bits[3], 0x80000000);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(test_int_zero) {
  s21_decimal dec;
  int res = s21_from_int_to_decimal(0, &dec);
  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(dec.bits[0], 0);
  ck_assert_uint_eq(dec.bits[1], 0);
  ck_assert_uint_eq(dec.bits[2], 0);
  ck_assert_uint_eq(dec.bits[3], 0);
}
END_TEST

START_TEST(test_int_one) {
  s21_decimal dec;
  int res = s21_from_int_to_decimal(1, &dec);
  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(dec.bits[0], 1);
  ck_assert_uint_eq(dec.bits[3], 0);
}
END_TEST

START_TEST(test_int_minus_one) {
  s21_decimal dec;
  int res = s21_from_int_to_decimal(-1, &dec);
  ck_assert_int_eq(res, 0);
  ck_assert_uint_eq(dec.bits[0], 1);
  ck_assert_uint_eq(dec.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_int_null_pointer) {
  int res = s21_from_int_to_decimal(42, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_int_roundtrip) {
  int values[] = {0, 1, -1, 42, -42, 1000000, -1000000, INT32_MAX, INT32_MIN};
  for (int i = 0; i < 9; i++) {
    s21_decimal dec;
    int dst;
    s21_from_int_to_decimal(values[i], &dec);
    s21_from_decimal_to_int(dec, &dst);
    ck_assert_int_eq(dst, values[i]);
  }
}
END_TEST

Suite* suite_from_int_to_decimal(void) {
  Suite* s = suite_create("suite_from_int_to_decimal");
  TCase* tc = tcase_create("core");

  tcase_add_test(tc, test_positive_ints);
  tcase_add_test(tc, test_negative_ints);
  tcase_add_test(tc, test_int_max);
  tcase_add_test(tc, test_int_min);
  tcase_add_test(tc, test_int_zero);
  tcase_add_test(tc, test_int_one);
  tcase_add_test(tc, test_int_minus_one);
  tcase_add_test(tc, test_int_null_pointer);
  tcase_add_test(tc, test_int_roundtrip);

  suite_add_tcase(s, tc);
  return s;
}