/*
 * A385080  Consecutive states of the linear congruential pseudo-random number
 *          generator 237*s mod (2^16+1) when started at s=1.
 * https://oeis.org/A385080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 237*a(n-1) mod 65537; a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;           /* a(1) = 1 */

    /* Compute 237^(n-1) mod 65537 */
    const seq_t MOD  = (seq_t)65537;
    const seq_t BASE = (seq_t)237;

    return mod_pow(BASE, n - 1, MOD);
}
