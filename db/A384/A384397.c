/*
 * A384397  Consecutive states of the linear congruential pseudo-random number
 *          generator 45742*s mod (2^31-909) when started at s=1.
 * https://oeis.org/A384397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45742 * a(n-1) mod (2^31-909), with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset, harmless */
    const seq_t MOD = (seq_t)2147482739ULL;/* 2^31 - 909 */
    return mod_pow((seq_t)45742, n-1, MOD);
}
