/*
 * A384398  Consecutive states of the linear congruential pseudo-random number
 *          generator 42024*s mod (2^31-847) when started at s=1.
 * https://oeis.org/A384398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 42024 * a(n-1) mod (2^31-847) (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t MOD = 2147482801ULL;  /* 2^31 - 847 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, guard */

    seq_t result = 1;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        uint64_t prod = (uint64_t)result * 42024ULL;   /* product fits in 64 bits */
        result = (seq_t)(prod % MOD);
    }
    return result;
}
