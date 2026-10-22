/*
 * A352059  a(n) = Sum_{p|n, p prime} (n-1)!/(n/p-1)!.
 * https://oeis.org/A352059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p prime divisor of n} (n-1)! / ((n/p)-1)!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return 0;                     /* sequence starts with 0 at a(1) */

    /* factorize n and collect distinct prime divisors */
    uint64_t primes[64];              /* enough for n <= 10k */
    uint64_t exps[64];                /* exponents are not needed here */
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;                    /* accumulator for the answer */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t start = (uint64_t)n / p;   /* integer division: n/p */
        seq_t prod = 1;                      /* product (n-1)! / ((n/p)-1)! */

        for (uint64_t k = start; k < n; ++k) {
            prod *= (seq_t)k;
        }
        sum += prod;
    }

    return sum;
}
