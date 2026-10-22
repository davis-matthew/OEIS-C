/*
 * A341596  Number of strictly inferior squarefree divisors of n.
 * https://oeis.org/A341596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n | d is squarefree and d² < n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/factor_u64.h"

/* Count squarefree divisors d of n with d < n/d (i.e. d² < n). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* a(1) = 0 */

    uint64_t nn = (uint64_t)n;

    /* Factor nn: primes[] holds distinct prime factors,
       exps[] holds corresponding exponents (unused). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* Compute floor(sqrt(nn - 1)) as the threshold for d. */
    uint64_t q = (uint64_t)floor(sqrt((long double)nn));
    uint64_t threshold = (q * q >= nn) ? (q > 0 ? q - 1 : 0) : q;

    seq_t result = 0;

    /* Enumerate all subsets of the distinct prime factors (squarefree divisors). */
    uint64_t mask_limit = (ptrdiff_t)1 << cnt;
    for (uint64_t mask = 0; mask < mask_limit; ++mask) {
        uint64_t prod = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                prod *= primes[i];
                if (prod > threshold) break;   /* early out if already too large */
            }
        }
        if (prod <= threshold) result++;
    }

    return result;
}
