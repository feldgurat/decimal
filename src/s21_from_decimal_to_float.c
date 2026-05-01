#include <math.h>

#include "s21_common.h"
#include "s21_decimal.h"
/*
В float

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

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  if (dst == S21_NULL) return 1;

  int error = 0;
  long double temp = 0;

  for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    temp += (long double)src.bits[i] * ldexpl(1.0L, i * 32);
  }

  int scale = s21_get_exponent(&src);
  int sign = s21_get_sign(&src);

  for (int i = 0; i < scale; i++) temp /= 10.0L;

  if (isnan((float)temp) || isinf((float)temp)) {
    error = 1;
  } else {
    if (sign) temp = -temp;
    *dst = (float)temp;
  }

  return error;
}
