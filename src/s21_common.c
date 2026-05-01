#include "s21_common.h"

int s21_get_exponent(const s21_decimal* dcm) {
  return (dcm->bits[3] & S21_EXP_MASK) >> 16;
}

int s21_get_sign(const s21_decimal* dcm) {
  return (dcm->bits[3] & S21_SIGN_MASK) >> 31;
}

int s21_set_sign(int sign, s21_decimal* dcm) {
  if (sign != PLUS && sign != MINUS) return 1;
  dcm->bits[3] = (dcm->bits[3] & S21_EXP_MASK) | ((uint32_t)sign << 31);
  return 0;
}

int s21_set_exponent(int n, s21_decimal* dcm) {
  if (n < 0 || n > 28) {
    return 1;
  }
  dcm->bits[3] = (dcm->bits[3] & S21_SIGN_MASK) | (n << 16);
  return 0;
}

int s21_mul_mantiss(s21_decimal* src, uint32_t n, s21_decimal* res) {
  uint64_t m = 0;
  for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    uint64_t t = (uint64_t)src->bits[i] * n + m;
    res->bits[i] = (uint32_t)t;
    m = t >> 32;
  }
  return (m != 0);
}

int s21_add_mantiss(const s21_decimal* a, const s21_decimal* b,
                    s21_decimal* res) {
  uint64_t m = 0;
  for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    uint64_t t = (uint64_t)a->bits[i] + (uint64_t)b->bits[i] + m;
    res->bits[i] = (uint32_t)t;
    m = t >> 32;
  }
  return (m != 0);
}

void s21_sub_mantiss(const s21_decimal* a, const s21_decimal* b,
                     s21_decimal* res) {
  uint64_t m = 0;
  for (int i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    uint64_t t = (uint64_t)a->bits[i] - (uint64_t)b->bits[i] - m;
    res->bits[i] = (uint32_t)t;
    m = (t >> 32) & 1;
  }
}

int s21_compare_mantiss(const s21_decimal* a, const s21_decimal* b) {
  int result = 0;
  for (int i = NUMBER_OF_SEGMENTS - 1; i >= 0 && result == 0; i--) {
    if (a->bits[i] != b->bits[i]) {
      result = (a->bits[i] > b->bits[i]) ? 1 : -1;
    }
  }
  return result;
}

int s21_is_zero(const s21_decimal* value) {
  return (value->bits[0] == 0 && value->bits[1] == 0 && value->bits[2] == 0);
}

int s21_count_digits(const s21_decimal* value) {
  s21_decimal tmp = *value;
  tmp.bits[3] = 0;
  int count = 0;
  int remainder;
  while (tmp.bits[0] || tmp.bits[1] || tmp.bits[2]) {
    s21_divide_mantiss(&tmp, 10, &remainder, 0);
    count++;
  }
  return count ? count : 1;
}

void s21_divide_mantiss(s21_decimal* value, uint32_t n, int* remainder,
                        uint64_t carry) {
  uint64_t r = carry;
  for (int i = NUMBER_OF_SEGMENTS - 1; i >= 0; i--) {
    uint64_t t = (r << 32) | (uint32_t)value->bits[i];
    value->bits[i] = (uint32_t)(t / n);
    r = t % n;
  }
  *remainder = (int)r;
}

void s21_normalize(s21_decimal* value_1, s21_decimal* value_2) {
  int exp1 = s21_get_exponent(value_1);
  int exp2 = s21_get_exponent(value_2);
  s21_decimal *smaller, *larger;
  int target_exp;
  if (exp1 < exp2) {
    smaller = value_1;
    larger = value_2;
    target_exp = exp2;
  } else {
    smaller = value_2;
    larger = value_1;
    target_exp = exp1;
  }

  int cur_exp = (exp1 < exp2) ? exp1 : exp2;

  int overflow = 0;
  while (cur_exp < target_exp && cur_exp <= 28 && !overflow) {
    s21_decimal backup = *smaller;
    if (s21_mul_mantiss(smaller, 10, smaller)) {
      *smaller = backup;
      overflow = 1;
    } else {
      cur_exp++;
    }
  }
  int sticky = 0;
  int remainder = 0;
  while (target_exp > cur_exp) {
    if (remainder) sticky = 1;
    s21_divide_mantiss(larger, 10, &remainder, 0);
    target_exp--;
  }
  if (remainder > 5 || (remainder == 5 && (sticky || (larger->bits[0] & 1u)))) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add_mantiss(larger, &one, larger);
  }

  s21_set_exponent(cur_exp, smaller);
  s21_set_exponent(target_exp, larger);
}

// делит на 10 И округляет
void s21_divide_mantiss_by_10_rounded(s21_decimal* value) {
  int remainder;
  s21_divide_mantiss(value, 10, &remainder, 0);
  s21_apply_bank_rounding(value, remainder);
}

// только округляет по уже готовому остатку
void s21_apply_bank_rounding(s21_decimal* value, int remainder) {
  if (remainder > 5) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add_mantiss(value, &one, value);
  } else if (remainder == 5) {
    s21_decimal temp = *value;
    int last;
    s21_divide_mantiss(&temp, 10, &last, 0);
    if (last % 2 == 1) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add_mantiss(value, &one, value);
    }
  }
}