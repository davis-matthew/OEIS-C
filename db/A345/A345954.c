/*
 * A345954  a(n) is the number of ternary strings of length n with at least
 *          three 0's.
 * https://oeis.org/A345954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - (2^(n-3))*(n^2 + 3*n + 8) for n ≥ 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* If length < 3 no ternary string can contain three zeros */
    if (n < 3)
        return (seq_t)0;

    /* 3^n using the generic power helper */
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* 2^(n-3) using the same helper */
    seq_t pow2 = seq_pow((seq_t)2, n - 3);

    /* Polynomial n^2 + 3*n + 8 */
    seq_t ns   = (seq_t)n;
    seq_t poly = ns * ns + (seq_t)3 * ns + (seq_t)8;

    /* Final result: 3^n - 2^(n-3) * (n^2 + 3*n + 8) */
    seq_t result = pow3 - pow2 * poly;
    return result;
}
