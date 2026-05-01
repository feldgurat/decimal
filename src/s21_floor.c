#include "s21_common.h"
#include "s21_decimal.h"

/*
Округляет указанное Decimal число до ближайшего целого числа в сторону
отрицательной бесконечности.

Возвращаемое значение — код ошибки:

0 — OK;
1 — ошибка вычисления.
*/

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;
  *result = S21_DECIMAL_ZERO;
  s21_truncate(value, result);

  int sign = s21_get_sign(&value);

  if (sign == MINUS) {
    s21_decimal check;
    s21_sub(value, *result, &check);
    s21_set_sign(0, &check);
    s21_decimal zero = S21_DECIMAL_ZERO;
    if (s21_compare_mantiss(&check, &zero) != 0) {
      s21_decimal one;
      s21_from_int_to_decimal(1, &one);
      s21_sub(*result, one, result);
    }
  }

  return 0;
}