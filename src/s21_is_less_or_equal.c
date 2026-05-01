#include "s21_common.h"
#include "s21_decimal.h"

/*
Меньше или равно	val1 <= val2

Возвращаемое значение:

0 — FALSE;
1 — TRUE.
*/

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_greater(value_1, value_2);
}