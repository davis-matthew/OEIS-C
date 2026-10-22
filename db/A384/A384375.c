/*
 * A384375  Consecutive internal states of the linear congruential
 *          pseudo-random number generator 950706376*s mod (2^31-1) when
 *          started at s=1.
 * https://oeis.org/A384375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 950706376 * a(n-1) mod (2^31-1) for n > 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

static const seq_t MODULUS     = (seq_t)2147483647ULL;   /* 2^31-1 */
static const seq_t MULTIPLIER  = (seq_t)950706376ULL;

seq_t a(idx_t n)
{
    /* The offset starts at 1, so any n < 1 is out of range; return 0. */
    if (n < 1)
        return (seq_t)0;

    /* a(1) is explicitly 1. */
    if (n == 1)
        return (seq_t)1;

    /* For n > 1, a(n) = MULTIPLIER^(n-1) mod MODULUS. */
    return mod_pow(MULTIPLIER, (uint64_t)(n - 1), MODULUS);
}
