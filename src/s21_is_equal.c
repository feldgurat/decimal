#include "s21_common.h"
#include "s21_decimal.h"

/*
Равно	val1 == val2

Возвращаемое значение:

0 — FALSE;
1 — TRUE.
*/

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (s21_is_zero(&value_1) && s21_is_zero(&value_2)) {
    result = 1;
  } else if (s21_get_sign(&value_1) == s21_get_sign(&value_2)) {
    s21_decimal v1 = value_1;
    s21_decimal v2 = value_2;
    s21_normalize(&v1, &v2);
    result = (s21_compare_mantiss(&v1, &v2) == 0) ? 1 : 0;
  }
  return result;
}