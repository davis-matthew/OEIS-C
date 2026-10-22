/*
 * A361793  Sum of the squares d^2 of the divisors d satisfying d^3|n.
 * https://oeis.org/A361793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d^3|n} d^2
 * Range:     u8 n <= 3374, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Multiply seq_t values (works for all supported seq_t types). */
static seq_t mul(seq_t x, seq_t y) {
    return x * y;
}

/* Compute 1 + p^2 + p^4 + ... + p^{2k};
   this is the value of (p^(2*(k+1)) - 1)/(p^2 - 1) without large powers. */
static seq_t geo_series_sum(seq_t p, uint64_t k) {
    seq_t sum = 1;
    seq_t term = 1;            /* term = p^{2*i} */
    for (uint64_t i = 1; i <= k; ++i) {
        term = mul(term, p);
        term = mul(term, p);   /* term = term * p^2 */
        sum = mul(sum, 1) + term; /* sum += term */
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for n < 1, return 0 for safety */

    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e   = exps[i];
        uint64_t k   = e / 3;          /* floor(e/3) */
        if (k == 0) continue;         /* contributes only the factor 1 */
        seq_t p = (seq_t)primes[i];
        seq_t part = geo_series_sum(p, k);
        result = mul(result, part);
    }
    return result;
}
