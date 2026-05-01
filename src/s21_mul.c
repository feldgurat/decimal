#include "s21_common.h"
#include "s21_decimal.h"

/*
Умножение	*

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

static void prod_div10(uint32_t* prod, int size, int* remainder, int* sticky) {
  if (*remainder) *sticky = 1;
  uint64_t r = 0;
  for (int i = size - 1; i >= 0; i--) {
    uint64_t t = (r << 32) | prod[i];
    prod[i] = (uint32_t)(t / 10);
    r = t % 10;
  }
  *remainder = (int)r;
}

static void prod_bank_round(uint32_t* prod, int size, int remainder,
                            int sticky) {
  if (remainder > 5 || (remainder == 5 && (sticky || (prod[0] & 1)))) {
    uint64_t c = 1;
    for (int i = 0; i < size && c; i++) {
      uint64_t t = (uint64_t)prod[i] + c;
      prod[i] = (uint32_t)t;
      c = t >> 32;
    }
  }
}

static int prod_has_overflow(const uint32_t* prod) {
  return (prod[3] || prod[4] || prod[5]);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == S21_NULL) return 1;
  *result = S21_DECIMAL_ZERO;

  int error = 0;
  int sign1 = s21_get_sign(&value_1);
  int sign2 = s21_get_sign(&value_2);
  int result_sign = (sign1 != sign2) ? MINUS : PLUS;
  int result_exp = s21_get_exponent(&value_1) + s21_get_exponent(&value_2);

  s21_set_sign(PLUS, &value_1);
  s21_set_sign(PLUS, &value_2);
  s21_set_exponent(0, &value_1);
  s21_set_exponent(0, &value_2);

  if (!s21_is_zero(&value_1) && !s21_is_zero(&value_2)) {
    uint32_t prod[6] = {0};
    for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
      uint64_t carry = 0;
      for (int j = 0; j < NUMBER_OF_SEGMENTS; j++) {
        uint64_t t =
            (uint64_t)value_1.bits[i] * value_2.bits[j] + prod[i + j] + carry;
        prod[i + j] = (uint32_t)t;
        carry = t >> 32;
      }
      prod[i + NUMBER_OF_SEGMENTS] += (uint32_t)carry;
    }

    int sticky = 0;
    int remainder = 0;
    while (prod_has_overflow(prod) && result_exp > 0) {
      prod_div10(prod, 6, &remainder, &sticky);
      result_exp--;
    }
    if (prod_has_overflow(prod)) {
      error = result_sign ? 2 : 1;
    } else {
      prod_bank_round(prod, 6, remainder, sticky);
      sticky = 0;
      remainder = 0;
      while (result_exp > 28) {
        prod_div10(prod, 3, &remainder, &sticky);
        result_exp--;
      }
      prod_bank_round(prod, 3, remainder, sticky);

      result->bits[0] = prod[0];
      result->bits[1] = prod[1];
      result->bits[2] = prod[2];
      s21_set_exponent(result_exp, result);
      s21_set_sign(result_sign, result);
    }
  }

  return error;
}