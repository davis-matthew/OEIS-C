/*
 * A362636  a(n) = Sum_{d|n, gcd(d,n/d)=1} d^d.
 * https://oeis.org/A362636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1} d^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) sums d^d over all unitary divisors d of n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow_seq.h"    /* pow_seq(seq_t, int) */

seq_t a(idx_t n)
{
    /* Factor n: obtain distinct primes and their exponents. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 0;
    /* Iterate over all subsets of the prime factors (unitary divisors). */
    uint64_t subsets = ((uint64_t)1) << cnt;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        seq_t d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                /* Multiply by the full prime power p^e. */
                d = d * pow_seq(pr[i], (int)ex[i]);
            }
        }
        /* Add d^d to the sum.  d fits in int for n up to ~10^4. */
        seq_t term = pow_seq(d, (int)d);
        result += term;
    }
    return result;
}
