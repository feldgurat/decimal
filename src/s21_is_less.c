#include "s21_common.h"
#include "s21_decimal.h"

/*
Меньше	val1 < val2

Возвращаемое значение:

0 — FALSE;
1 — TRUE.
*/

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (!(s21_is_zero(&value_1) && s21_is_zero(&value_2))) {
    int sign1 = s21_get_sign(&value_1);
    int sign2 = s21_get_sign(&value_2);
    if (sign1 != sign2) {
      result = sign1 ? 1 : 0;
    } else {
      s21_decimal v1 = value_1;
      s21_decimal v2 = value_2;
      s21_normalize(&v1, &v2);
      int cmp = s21_compare_mantiss(&v1, &v2);
      result = sign1 ? (cmp > 0 ? 1 : 0) : (cmp < 0 ? 1 : 0);
    }
  }
  return result;
}