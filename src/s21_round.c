#include "s21_common.h"
#include "s21_decimal.h"

/*
Округляет Decimal до ближайшего целого числа.

Возвращаемое значение — код ошибки:

0 — OK;
1 — ошибка вычисления.
*/

int s21_round(s21_decimal value, s21_decimal* result) {
  if (result == S21_NULL) return 1;
  *result = S21_DECIMAL_ZERO;

  int sign = s21_get_sign(&value);
  int exp = s21_get_exponent(&value);

  if (exp == 0) {
    *result = value;
  } else {
    s21_decimal tmp = value;
    s21_set_sign(PLUS, &tmp);

    int remainder = 0;
    while (exp > 0) {
      s21_divide_mantiss(&tmp, 10, &remainder, 0);
      exp--;
    }

    if (remainder >= 5) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add_mantiss(&tmp, &one, &tmp);
    }

    s21_set_exponent(0, &tmp);
    s21_set_sign(sign, &tmp);
    *result = tmp;
  }

  return 0;
}