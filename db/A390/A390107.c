/*
 * A390107  Least positive integer m such that m*(n-1) + 1 is relatively prime
 *          to C(m*n^2, m*n), where C(k, j) denotes the binomial coefficient
 *          k!/(j!*(k-j)!).
 * https://oeis.org/A390107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least m≥1 such that gcd(m*(n‑1)+1 , C(m*n² , m*n)) = 1. */
/* The algorithm checks each candidate m by factoring the modulus
   m*(n‑1)+1 and ensuring none of its prime factors divide the
   binomial coefficient, computed via exponent counting in factorials.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static uint64_t fact_exp(uint64_t x, uint64_t p)
{
    uint64_t e = 0;
    while (x) {
        x /= p;
        e += x;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Special case n = 1 (modulus = 1). */
    if (n == 1) return (seq_t)1;

    /* Start searching for the minimal m. */
    for (uint64_t m = 1; ; ++m)
    {
        uint64_t mod = m * ((uint64_t)n - 1) + 1;

        /* Factor the modulus. */
        uint64_t primes[64];
        uint64_t exps[64];   /* not used, but required by the helper */
        size_t   cnt = 0;
        factor_u64(mod, primes, exps, &cnt);

        bool bad = false;
        for (size_t i = 0; i < cnt && !bad; ++i)
        {
            uint64_t p = primes[i];

            /* Exponent of p in C(k,r) where k = m*n^2, r = m*n */
            uint64_t k = m * (uint64_t)n * (uint64_t)n;
            uint64_t r = m * (uint64_t)n;
            uint64_t e = fact_exp(k, p) - fact_exp(r, p) - fact_exp(k - r, p);

            if (e > 0)   /* p divides the binomial coefficient */
                bad = true;
        }

        if (!bad)
            return (seq_t)m;
    }
}
