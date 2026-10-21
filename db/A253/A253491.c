/*
 * A253491  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2802 + 397*2^n + 49*3^(n-1) for n ≥ 4; a(1)=3639,
 *            a(2)=5386, a(3)=6476
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3639;
    if (n == 2) return (seq_t)5386;
    if (n == 3) return (seq_t)6476;

    /* 2^n and 3^(n-1) */
    seq_t pow2 = seq_pow((seq_t)2, n);
    seq_t pow3 = seq_pow((seq_t)3, n - 1);

    /* Multiply by the constants */
    pow2 *= (seq_t)397;
    pow3 *= (seq_t)49;

    /* Assemble the result */
    seq_t res = pow2 + pow3;
    res += (seq_t)2802;
    return res;
}
