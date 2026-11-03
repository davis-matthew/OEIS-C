/*
 * A347089  a(n) = gcd(A055155(n), d(n)), where A055155(n) = Sum_{d|n} gcd(d,
 *          n/d) and d(n) gives the number of divisors of n.
 * https://oeis.org/A347089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A055155(n), d(n)) * where A055155(n) = Σ_{d|n} gcd(d,
 *            n/d) and d(n) is the number of divisors of n
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9421 (2026-11-01)
 * Helpers:   factor_u64, gcd, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/gcd_u64.h"

/* Recursive helper that sums gcd(d, n/d) over all divisors d of n.
 * cur is the current divisor value (type idx_t).
 * sum is updated by reference (type uint64_t). */
static void _sum_divisor_gcd(size_t idx, idx_t cur, idx_t n,
                             const uint64_t *primes, const uint64_t *exps,
                             size_t cnt, uint64_t *sum)
{
    if (idx == cnt) {
        uint64_t other = (uint64_t)n / (uint64_t)cur;
        *sum += gcd_u64((uint64_t)cur, other);
        return;
    }

    uint64_t prime = primes[idx];
    idx_t cur_power = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        _sum_divisor_gcd(idx + 1, cur_power, n, primes, exps, cnt, sum);
        cur_power *= prime;
    }
}
                                 /*               uint64_t *pr,      */
                                 /*               uint64_t *ex,     */
                                 /*               size_t *cnt);     */

/* Small helper: gcd for uint64_t values. */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n = 0, guard anyway */

    /* Factorise n */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute d(n) = product (exp + 1) */
    uint64_t divcnt = 1;
    for (size_t i = 0; i < cnt; ++i)
        divcnt *= (exps[i] + 1);

    /* Compute A055155(n) = Σ_{d|n} gcd(d, n/d) */
    uint64_t sum = 0;
    _sum_divisor_gcd(0, (idx_t)1, n, primes, exps, cnt, &sum);

    /* a(n) = gcd(sum, divcnt) */
    uint64_t g = gcd_u64(sum, divcnt);
    return (seq_t)g;
}
