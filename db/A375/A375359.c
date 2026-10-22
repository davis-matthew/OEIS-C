/*
 * A375359  The maximum exponent in the prime factorization of the smallest
 *          number whose square is divisible by n.
 * https://oeis.org/A375359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(A051903(n)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v(n) = maximum exponent in the prime factorization of n; a(n) = ceil(v(n)/2). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle n=1 explicitly – factor_u64 will return zero factors anyway */
    if (n == 0) return 0;   /* undefined for OEIS but harmless */

    uint64_t uu = (uint64_t)n;      /* n fits in uint64_t (idx_t) */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];

    factor_u64(uu, primes, exps, &cnt);

    uint64_t maxe = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > maxe)
            maxe = exps[i];

    /* ceil(maxe / 2) = (maxe + 1) / 2 */
    seq_t result = (seq_t)((maxe + 1) / 2);
    return result;
}
