/*
 * A274846  Numbers described with a single word in Russian.
 * https://oeis.org/A274846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, word
 * Formula:   a(n) = n-1 for 1 ≤ n ≤ 21 a(n) = 10*(n-21)+20 for 22 ≤ n ≤ 28
 *            a(n) = 100*(n-28) for 29 ≤ n ≤ 37 a(38) = 1000 a(n) =
 *            10^(6+3*(n-39)) for n ≥ 39 (powers of ten with exponents
 *            6,9,12,...)
 * Range:     u8 n <= 30, u16 n <= 38, u32 n <= 40, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 21)
        return (seq_t)(n - 1);

    if (n <= 28)
    {
        /* 10*(n-21)+20 */
        idx_t t = n - 21;            /* 1…7 */
        return (seq_t)(10 * t + 20);
    }

    if (n <= 37)
    {
        /* 100*(n-28) */
        idx_t t = n - 28;            /* 1…9 */
        return (seq_t)(100 * t);
    }

    if (n == 38)
        return (seq_t)1000;

    /* n ≥ 39: powers of ten with exponent 6 + 3*(n-39) */
    idx_t e = 6 + 3 * (n - 39);
    return pow10(e);
}
