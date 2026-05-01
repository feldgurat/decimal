#ifndef S21_COMMON_H
#define S21_COMMON_H
#include <stdint.h>
#include <string.h>

#include "s21_decimal.h"

int s21_get_exponent(const s21_decimal* dcm);
int s21_get_sign(const s21_decimal* dcm);
int s21_set_sign(int sign, s21_decimal* dcm);
int s21_set_exponent(int n, s21_decimal* dcm);

int s21_mul_mantiss(s21_decimal* src, uint32_t n, s21_decimal* res);
int s21_add_mantiss(const s21_decimal* a, const s21_decimal* b,
                    s21_decimal* res);
void s21_sub_mantiss(const s21_decimal* a, const s21_decimal* b,
                     s21_decimal* res);
int s21_compare_mantiss(const s21_decimal* a, const s21_decimal* b);
int s21_is_zero(const s21_decimal* value);

void s21_normalize(s21_decimal* value_1, s21_decimal* value_2);
void s21_divide_mantiss_by_10_rounded(s21_decimal* value);
void s21_apply_bank_rounding(s21_decimal* value, int remainder);
void s21_divide_mantiss(s21_decimal* value, uint32_t n, int* remainder,
                        uint64_t carry);
int s21_count_digits(const s21_decimal* value);
#endif