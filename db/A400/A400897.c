/*
 * A400897  a(n) = denominator(1 - (n^2 - n + 2)/2^n).
 * https://oeis.org/A400897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, new
 * Formula:   a(n) = denominator(1 - (n^2 - n + 2)/2^n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t s  = nn * nn - nn + (seq_t)2;   /* n^2 - n + 2 */

    /* exponent of the largest power of two dividing s */
    idx_t v = 0;
    while ((s & (seq_t)1) == 0) {
        s >>= 1;
        v++;
    }

    idx_t exp = (idx_t)n - v;            /* remaining 2‑power exponent */
    return pow2(exp);
}
