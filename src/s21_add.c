#include "s21_common.h"
#include "s21_decimal.h"

/*
Сложение	+

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

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == S21_NULL) return 1;

  *result = S21_DECIMAL_ZERO;
  int error = 0;
  int sign1 = s21_get_sign(&value_1);
  int sign2 = s21_get_sign(&value_2);

  if (sign1 != sign2) {
    if (sign1) {
      s21_set_sign(0, &value_1);
      error = s21_sub(value_2, value_1, result);
    } else {
      s21_set_sign(0, &value_2);
      error = s21_sub(value_1, value_2, result);
    }
  } else {
    s21_decimal v1 = value_1;
    s21_decimal v2 = value_2;
    s21_normalize(&v1, &v2);
    int exp = s21_get_exponent(&v1);
    s21_decimal sum = S21_DECIMAL_ZERO;
    int overflow = s21_add_mantiss(&v1, &v2, &sum);

    if (overflow) {
      if (exp == 0) {
        error = sign1 ? 2 : 1;
      } else {
        int remainder;
        s21_divide_mantiss(&sum, 10, &remainder, 1);
        s21_apply_bank_rounding(&sum, remainder);
        exp--;
      }
    }

    if (error == 0) {
      s21_set_exponent(exp, &sum);
      s21_set_sign(sign1, &sum);
      *result = sum;
    }
  }

  return error;
}