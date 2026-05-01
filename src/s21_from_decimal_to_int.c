#include <limits.h>

#include "s21_common.h"
#include "s21_decimal.h"

/*
В int

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

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  if (dst == S21_NULL) return 1;

  *dst = 0;
  int error = 0;
  int sign = s21_get_sign(&src);

  s21_decimal truncated = src;
  int exp = s21_get_exponent(&truncated);
  for (int i = 0; i < exp; i++) {
    int remainder;
    s21_divide_mantiss(&truncated, 10, &remainder, 0);
  }
  s21_set_exponent(0, &truncated);

  if (!s21_is_zero(&src)) {
    if (truncated.bits[2] != 0) {
      error = 1;
    } else {
      uint64_t value =
          (uint64_t)truncated.bits[1] << 32 | (uint64_t)truncated.bits[0];
      if (sign == PLUS) {
        if (value > 0x7FFFFFFF) {
          error = 1;
        } else {
          *dst = (int)value;
        }
      } else {
        if (value > 0x80000000) {
          error = 1;
        } else if (value == (uint64_t)0x80000000u) {
          *dst = INT_MIN;
        } else {
          *dst = -(int)(uint32_t)value;
        }
      }
    }
  }

  return error;
}