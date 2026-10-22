/*
 * A384400  Consecutive states of the linear congruential pseudo-random number
 *          generator 40692*s mod (2^31-249) when started at s=1.
 * https://oeis.org/A384400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40692 * a(n-1) mod (2^31-249), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 40692^(n-1) mod (2^31-249) */
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    const seq_t k = (seq_t)40692;
    const seq_t m = (seq_t)2147483399; /* 2^31 - 249 */
    return mod_pow(k, (uint64_t)(n-1), m);
}
