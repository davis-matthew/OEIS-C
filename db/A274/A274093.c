/*
 * A274093  a(0)=0; thereafter (-1)^n*n appears n times.
 * https://oeis.org/A274093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^k * k, where k is the least integer with k*(k+1)/2
 *            >= n, for n >= 1; a(0) = 0. This implements the definition that
 *            (-1)^n * n appears n times
 * Range:     i8 n <= 8128, i16 n <= 9269, i32 n <= 9269, i64 n <= 9269,
 *            i128 n <= 9269, big any n
 * Helpers:   integer_sqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/integer_sqrt128.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned __int128 rhs = (unsigned __int128)n * 8 + 1;
    unsigned long long sqrt_val = integer_sqrt128(rhs);
    unsigned long long k = (sqrt_val - 1) >> 1;          /* floor of the formula */

    unsigned long long t = k * (k + 1) / 2;              /* triangular number */
    if (t < n)
        ++k;                                            /* adjust to ceil */

    /* sign: (-1)^k * k */
    if (k & 1ULL)
        return (seq_t)(-(seq_t)k);
    else
        return (seq_t)k;
}
