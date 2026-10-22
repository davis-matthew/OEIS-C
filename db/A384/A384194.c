/*
 * A384194  Consecutive states of the linear congruential pseudo-random number
 *          generator 259*s mod 2^15 when started at s=1.
 * https://oeis.org/A384194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 259 * a(n-1) mod 2^15
 * Range:     u8 none, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 2^15 = 32768 */
    const seq_t MOD = (seq_t)(1ULL << 15);
    /* Compute 259^(n-1) mod MOD; works for all n >= 1 */
    return mod_pow((seq_t)259, (uint64_t)(n - 1), MOD);
}
