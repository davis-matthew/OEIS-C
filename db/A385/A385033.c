/*
 * A385033  Consecutive states of the linear congruential pseudo-random number
 *          generator 170*s mod 30323 when started at s=1.
 * https://oeis.org/A385033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 170 * a(n-1) mod 30323, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined below offset */
    if (n == 1) return 1;               /* initial value */
    /* Compute 170^(n-1) mod 30323 */
    seq_t base = 170;
    seq_t mod  = 30323;
    return mod_pow(base, n-1, mod);
}
