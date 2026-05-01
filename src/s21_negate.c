#include "s21_common.h"
#include "s21_decimal.h"

/*
Возвращает результат умножения указанного Decimal на -1.

Возвращаемое значение — код ошибки:

0 — OK;
1 — ошибка вычисления.
*/

int s21_negate(s21_decimal value, s21_decimal* result) {
  if (result == S21_NULL) return 1;
  *result = value;

  int sign = s21_get_sign(&value);
  s21_set_sign(!sign, result);

  return 0;
}