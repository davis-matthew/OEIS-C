/*
 * A326987  Number of nonpowers of 2 dividing n.
 * https://oeis.org/A326987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) - (v2(n)+1)
 * Range:     u8 n <= 8988, u16 n <= 8988, u32 n <= 8988, u64 n <= 8988,
 *            u128 n <= 8988, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where d(n) = number of divisors of n */
/*   and v2(n) = exponent of 2 in n */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* exponent of 2 in n */
    unsigned v2 = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0 && t != 0) {
        t >>= 1;
        ++v2;
    }

    /* factorize n and compute divisor count */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        d = d * (seq_t)(exps[i] + 1);
    }

    /* a(n) = d(n) - (v2(n)+1) */
    seq_t result = d - (seq_t)(v2 + 1);
    return result;
}
