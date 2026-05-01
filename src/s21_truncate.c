#include "s21_common.h"
#include "s21_decimal.h"

/*
Возвращает целые цифры указанного Decimal числа; любые дробные цифры
отбрасываются, включая конечные нули.

Возвращаемое значение — код ошибки:

0 — OK;
1 — ошибка вычисления.
*/

int s21_truncate(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  *result = value;

  int scale = s21_get_exponent(result);

  while (scale > 0) {
    int remainder;
    s21_divide_mantiss(result, 10, &remainder, 0);
    scale--;
  }

  s21_set_exponent(0, result);

  return 0;
}