#include "s21_common.h"
#include "s21_decimal.h"

/*
Вычитание	-

Функции возвращают код ошибки:

0 — OK;
1 — число слишком велико или равно бесконечности;
2 — число слишком мало или равно отрицательной бесконечности;
3 — деление на 0.
Уточнение про числа, не вмещающиеся в мантиссу:

При получении чисел, не вмещающихся в мантиссу при арифметических операциях,
используй банковское округление
(например, 79,228,162,514,264,337,593,543,950,335 — 0.6 =
79,228,162,514,264,337,593,543,950,334).
*/

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == S21_NULL) return 1;

  *result = S21_DECIMAL_ZERO;
  int error = 0;
  int sign1 = s21_get_sign(&value_1);
  int sign2 = s21_get_sign(&value_2);

  if (sign1 != sign2) {
    s21_set_sign(sign1, &value_2);
    error = s21_add(value_1, value_2, result);
  } else {
    s21_decimal v1 = value_1;
    s21_decimal v2 = value_2;
    s21_normalize(&v1, &v2);
    int cmp = s21_compare_mantiss(&v1, &v2);

    if (cmp != 0) {
      s21_decimal *larger, *smaller;
      if (cmp > 0) {
        larger = &v1;
        smaller = &v2;
      } else {
        larger = &v2;
        smaller = &v1;
      }
      s21_decimal diff = S21_DECIMAL_ZERO;
      s21_sub_mantiss(larger, smaller, &diff);
      s21_set_exponent(s21_get_exponent(&v1), &diff);
      s21_set_sign((cmp > 0) ? sign1 : !sign1, &diff);
      *result = diff;
    }
  }

  return error;
}