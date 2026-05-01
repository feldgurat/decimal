#include <math.h>
#include <stdio.h>

#include "s21_common.h"
#include "s21_decimal.h"

/*
Из float

Возвращаемое значение — код ошибки:

0 — OK;
1 — ошибка конвертации.
Уточнение про преобразование числа типа float:

Если числа слишком малы (0 < |x| < 1e-28), возвращай ошибку и значение, равное
0. Если числа слишком велики (|x| > 79,228,162,514,264,337,593,543,950,335) или
равны бесконечности, возвращай ошибку. При обработке числа с типом float
преобразовывай все содержащиеся в нём значимые десятичные цифры. Если таких цифр
больше 7, то значение числа должно округляться с использованием банковского
округления к числу, у которого не больше 7 значимых цифр. Уточнение про
преобразование из числа типа decimal в тип int:

Если в числе типа decimal есть дробная часть, то её следует отбросить (например,
0.9 преобразуется 0).
*/

static int convert_large_exp(s21_decimal* dst, int n) {
  int error = 0;
  for (int j = 0; j < n && !error; j++) {
    uint32_t acc = 0;
    for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
      uint32_t new_acc = dst->bits[i] >> 31;
      dst->bits[i] = (dst->bits[i] << 1) | acc;
      acc = new_acc;
    }
    if (acc != 0) {
      error = 1;
    }
  }
  return error;
}

static int convert_small_exp(s21_decimal* dst, int positive_n) {
  int error = 0;
  for (int i = 0; i < positive_n && !error; i++) {
    if (s21_mul_mantiss(dst, 5, dst)) {
      *dst = S21_DECIMAL_ZERO;
      error = 1;
    } else {
      int exp = s21_get_exponent(dst) + 1;
      while (s21_count_digits(dst) > 7 || exp > 28) {
        s21_divide_mantiss_by_10_rounded(dst);
        exp--;
      }
      if (exp < 0 || s21_is_zero(dst)) {
        *dst = S21_DECIMAL_ZERO;
        error = 1;
      } else {
        s21_set_exponent(exp, dst);
      }
    }
  }
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  if (dst == S21_NULL) return 1;

  *dst = S21_DECIMAL_ZERO;
  int error = 0;
  uint32_t raw;
  memcpy(&raw, &src, sizeof(float));
  int sign = (raw >> 31) & 1;
  int raw_exp = (raw >> 23) & 0xFF;
  uint32_t raw_mantiss = raw & 0x7FFFFF;

  if (raw_exp == 0 && raw_mantiss != 0) {
    error = 1;  // денормализованное
  } else if (raw_exp == 255) {
    error = 1;  // inf или NaN
  } else if (raw_exp != 0 || raw_mantiss != 0) {
    int bin_exp = raw_exp - 127;
    uint32_t mantiss = raw_mantiss | 0x800000;
    s21_from_int_to_decimal(mantiss, dst);
    int n = bin_exp - 23;

    if (n >= 0) {
      error = convert_large_exp(dst, n);
    } else {
      error = convert_small_exp(dst, -n);
    }

    if (!error) {
      s21_set_sign(sign, dst);
    }
  }

  return error;
}