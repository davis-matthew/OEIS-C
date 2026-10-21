/*
 * A247473  Numbers of the form 2^k (k>=0) that are a sum of divisors of m for
 *          some m.
 * https://oeis.org/A247473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=8, a(4)=32, a(5)=128, a(6)=256, a(7)=512,
 *            a(8)=1024; for n >= 9: a(n) = 2^(n+3)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for non‑positive indices */
    if (n <= 8) {
        static const idx_t exps[8] = {0, 2, 3, 5, 7, 8, 9, 10};
        return pow2(exps[n-1]);    /* 2^exponent */
    }
    return pow2(n + 3);             /* 2^(n+3) for larger n */
}
