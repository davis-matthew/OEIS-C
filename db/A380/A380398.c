/*
 * A380398  The number of unitary divisors of n that are perfect powers
 *          (A001597).
 * https://oeis.org/A380398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, gcd(d, n/d) == 1} [d in A001597]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // no term before offset

    uint64_t nn = (uint64_t)n;       // factor_u64 expects uint64_t
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t count = 1;                 // unitary divisor 1 is always a perfect power

    /* iterate over all non‑empty subsets of the distinct prime factors */
    for (size_t mask = 1; mask < ((size_t)1 << cnt); ++mask) {
        int g = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (mask & ((size_t)1 << j)) {
                int e = (int)exps[j];
                if (g == 0)
                    g = e;
                else {
                    g = gcd_int(g, e);
                    if (g == 1) break;   /* cannot become a perfect power */
                }
            }
        }
        if (g > 1)
            ++count;
    }

    return count;
}
