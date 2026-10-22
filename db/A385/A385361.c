/*
 * A385361  Consecutive states of the linear congruential pseudo-random number
 *          generator (2661*s + 36979) mod 175000 when started at s=1.
 * https://oeis.org/A385361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2661 * a(n-1) + 36979) mod 175000, with a(1) = 1. The
 *            recurrence is computed by fast exponentiation
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lcg_pow_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* just in case */
#include "../utils/lcg_pow_sum.h"

seq_t a(idx_t n)
{
    const seq_t MOD = 175000;
    if (n < 1) return 0;          /* undefined for n<1; return 0 as safe fallback */
    if (n == 1) return 1;         /* first term */
    idx_t k = n - 1;               /* number of steps from initial state */
    seq_t pow, sum;
    lcg_pow_sum(k, &pow, &sum);
    seq_t result = (pow + sum) % MOD;   /* since a(1)=1, pow*1 = pow */
    return result;
}
