#include "s21_common.h"
#include "s21_decimal.h"

static uint32_t div_step(s21_decimal* remainder, const s21_decimal* divisor) {
  uint32_t digit = 0;
  if (s21_compare_mantiss(remainder, divisor) >= 0) {
    uint32_t lo = 1, hi = 9;
    digit = 1;
    while (lo <= hi) {
      uint32_t mid = (lo + hi) / 2;
      s21_decimal tmp;
      int overflow = s21_mul_mantiss((s21_decimal*)divisor, mid, &tmp);
      int cmp = overflow ? -1 : s21_compare_mantiss(remainder, &tmp);
      if (cmp >= 0) {
        digit = mid;
        lo = mid + 1;
      } else {
        hi = mid - 1;
      }
    }
    s21_decimal sub;
    s21_mul_mantiss((s21_decimal*)divisor, digit, &sub);
    s21_sub_mantiss(remainder, &sub, remainder);
  }
  return digit;
}

static int shift_and_add(s21_decimal* quotient, uint32_t digit) {
  int error = 0;
  s21_decimal tmp;
  if (s21_mul_mantiss(quotient, 10, &tmp)) {
    error = 1;
  } else {
    s21_decimal d = {{digit, 0, 0, 0}};
    if (s21_add_mantiss(&tmp, &d, &tmp)) {
      error = 1;
    } else {
      *quotient = tmp;
    }
  }
  return error;
}

static void shift_left(s21_decimal* val, int* overflow) {
  uint32_t carry = 0;
  *overflow = 0;
  for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    uint32_t new_carry = val->bits[i] >> 31;
    val->bits[i] = (val->bits[i] << 1) | carry;
    carry = new_carry;
  }
  if (carry) *overflow = 1;
}

static void shift_left_n(s21_decimal* val, int n, int* ok) {
  *ok = 1;
  for (int b = 0; b < n && *ok; b++) {
    int overflow;
    shift_left(val, &overflow);
    if (overflow) *ok = 0;
  }
}

static void integer_divide(s21_decimal* quotient, s21_decimal* remainder,
                           const s21_decimal* divisor) {
  for (int bit = 95; bit >= 0; bit--) {
    int dummy;
    shift_left(quotient, &dummy);
    s21_decimal shifted = *divisor;
    int ok;
    shift_left_n(&shifted, bit, &ok);
    if (ok && s21_compare_mantiss(remainder, &shifted) >= 0) {
      s21_sub_mantiss(remainder, &shifted, remainder);
      quotient->bits[0] |= 1;
    }
  }
}

static void fractional_divide(s21_decimal* quotient, s21_decimal* remainder,
                              const s21_decimal* divisor, int* result_exp) {
  int done = 0;
  int frac_digits = 0;
  while (!done && !s21_is_zero(remainder) && *result_exp < 28 &&
         frac_digits < 29) {
    if (s21_mul_mantiss(remainder, 10, remainder)) {
      done = 1;
    } else {
      uint32_t digit = div_step(remainder, divisor);
      if (shift_and_add(quotient, digit)) {
        s21_apply_bank_rounding(quotient, (int)digit);
        done = 1;
      } else {
        (*result_exp)++;
        frac_digits++;
      }
    }
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (result == S21_NULL) return 1;
  *result = S21_DECIMAL_ZERO;

  int error = 0;
  if (s21_is_zero(&value_2)) {
    error = 3;
  } else if (s21_is_zero(&value_1)) {
    error = 0;
  } else {
    int sign1 = s21_get_sign(&value_1);
    int sign2 = s21_get_sign(&value_2);
    int result_sign = (sign1 != sign2) ? MINUS : PLUS;
    int result_exp = s21_get_exponent(&value_1) - s21_get_exponent(&value_2);

    s21_set_sign(PLUS, &value_1);
    s21_set_sign(PLUS, &value_2);
    s21_set_exponent(0, &value_1);
    s21_set_exponent(0, &value_2);

    s21_decimal quotient = S21_DECIMAL_ZERO;
    s21_decimal remainder = value_1;

    integer_divide(&quotient, &remainder, &value_2);
    fractional_divide(&quotient, &remainder, &value_2, &result_exp);

    while (result_exp < 0 && error == 0) {
      if (s21_mul_mantiss(&quotient, 10, &quotient)) {
        error = result_sign ? 2 : 1;
      } else {
        result_exp++;
      }
    }

    while (result_exp > 28) {
      s21_divide_mantiss_by_10_rounded(&quotient);
      result_exp--;
    }

    if (error == 0) {
      s21_set_exponent(result_exp, &quotient);
      s21_set_sign(result_sign, &quotient);
      *result = quotient;
    }
  }

  return error;
}