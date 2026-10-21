/*
 * A248054  Least positive integer m such that m + n divides sigma(m^2) +
 *          sigma(n^2), where sigma(k) is the sum of all positive divisors of
 *          k.
 * https://oeis.org/A248054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m≥1 such that (m+n) divides σ(m²)+σ(n²)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 44, u64 n <= 3190,
 *            u128 n <= 3190, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility headers */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

/* Compute σ(m²) for a 64‑bit integer m */
static seq_t sigma_square_u64(uint64_t m)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t pseq = (seq_t)p;

        /* σ(p²ᵉ) = (p^(2e+1)-1)/(p-1) */
        seq_t numerator = seq_pow(pseq, (idx_t)(2*e + 1));
        numerator = numerator - 1;
        seq_t denominator = (seq_t)(p - 1);
        seq_t part = numerator / denominator;

        result = result * part;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;          /* n is 1‑based */
    seq_t sigma_n2 = sigma_square_u64(N);

    for (uint64_t m = 1;; ++m) {
        seq_t sigma_m2 = sigma_square_u64(m);
        seq_t sum = sigma_m2 + sigma_n2;
        seq_t d = (seq_t)m + (seq_t)N;  /* m + n */

        if ((sum % d) == 0)
            return (seq_t)m;
    }
}
