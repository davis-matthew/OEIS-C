/*
 * A328882  a(n) = n - 2^(sum of digits of n).
 * https://oeis.org/A328882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = n - 2^A007953(n) (A007953(n) = sum of base‑10 digits of
 *            n)
 * Range:     i8 n <= 7, i16 n <= 78, i32 n <= 5998, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Validated: standard, pass, 77 terms through n = 9932 (2026-11-01)
 * Helpers:   digit_sum_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum_idx.h"
#include "../utils/pow2.h"

/* external linkage: value type seq_t is supplied by the build */
seq_t a(idx_t n)
{
    idx_t sum = digit_sum_idx(n);
    seq_t pow = pow2(sum);        /* 2^sum of digits */
    return ((seq_t)n) - pow;      /* n - 2^sum */
}
