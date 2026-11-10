/*
 * A269862  Least monotonic left inverse of A269861.
 * https://oeis.org/A269862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(n) = ((A048673(n)-n) mod 2) + a(n-1)
 * Range:     u8 n <= 498, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9993 (2026-11-07)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Parity of A048673(m); the product is kept modulo 2^64, which preserves it mod 4. */
static unsigned A048673_parity(uint64_t m)
{
    if (m == 1) return 1u; /* A048673(1) = 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod *= np;
    }
    return (unsigned)(((prod + 1) >> 1) & 1u);
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (uint64_t m = 2; m <= (uint64_t)n; ++m) {
        unsigned pa = A048673_parity(m);
        unsigned pm = (unsigned)(m & 1u);
        if (pa != pm)
            ++total;
    }
    return total;
}
