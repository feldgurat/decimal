#include "s21_common.h"
#include "s21_decimal.h"
/*
Больше	val1 > val2

Возвращаемое значение:

0 — FALSE;
1 — TRUE.
*/

int s21_is_greater(s21_decimal val1, s21_decimal val2) {
  return s21_is_less(val2, val1);
}