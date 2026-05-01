#include "s21_decimal.h"

/*
Больше или равно	val1 >= val2

Возвращаемое значение:

0 — FALSE;
1 — TRUE.
*/

int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2) {
  return !s21_is_less(val1, val2);
}
