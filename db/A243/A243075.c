/*
 * A243075  Least number k such that k + DigitSum(k) = 10^n or 0 if no such
 *          number exists.
 * https://oeis.org/A243075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A243075: least k such that k + digit_sum(k) = 10^n, or 0 if no such k exists.
   Computed by brute‑force within the guaranteed range [10^n-9n, 10^n-1]. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t ten_pow = pow10(n);

    /* lower bound of the search interval: 10^n - 9*n */
    seq_t low = ten_pow - (seq_t)(9ULL * n);

    /* iterate over 9*n candidates */
    for (idx_t i = 0; i < 9ULL * n; ++i)
    {
        seq_t k = low + (seq_t)i;
        if (k + digit_sum(k) == ten_pow)
            return k;
    }
    return 0;   /* no solution */
}
