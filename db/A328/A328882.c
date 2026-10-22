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
 * Helpers:   partial_digit_sum, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/partial_digit_sum.h"

/* external linkage: value type seq_t is supplied by the build */
seq_t a(idx_t n)
{
    idx_t sum = partial_digit_sum(n);
    seq_t pow = pow2(sum);        /* 2^sum of digits */
    return ((seq_t)n) - pow;      /* n - 2^sum */
}
