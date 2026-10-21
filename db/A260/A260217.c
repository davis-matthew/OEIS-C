/*
 * A260217  Number of base-3 n-digit pandigital numbers.
 * https://oeis.org/A260217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*3^(n-1) - 2^(n+1) + 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 3^(n-1) */
    seq_t pow3 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow3 *= 3;

    /* 2^(n+1) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow2 *= 2;

    /* a(n) = 2*pow3 - pow2 + 2, avoiding unsigned underflow */
    seq_t t1 = pow3 * 2;
    seq_t t1_plus2 = t1 + 2;
    if (pow2 > t1_plus2)  /* true only for n=0, which is outside the offset */
        return 0;
    return t1_plus2 - pow2;
}
