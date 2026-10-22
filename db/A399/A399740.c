/*
 * A399740  The sum of the unitary divisors d of n such that n/d is a perfect
 *          power (A001597).
 * https://oeis.org/A399740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{d|_u n, n/d is a perfect power} d (sum of the unitary
 *            divisors d of n such that n/d is a perfect power)
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* gcd of two unsigned 64‑bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* a(n) as defined above */
seq_t a(idx_t n)
{
    /* 1 is a special case – its only unitary divisor is 1 itself. */
    if (n == 1)
        return (seq_t)1;

    /* factorise n;  factor_u64 expects a uint64_t argument. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If, for some reason, factorisation fails and returns cnt == 0,
       treat n as prime (unitary divisor 1 only).  This case does not
       occur for n > 1 with a correct factor_u64 implementation. */
    if (cnt == 0)
        return (seq_t)1 + (seq_t)primes[0];

    seq_t sum = 0;

    /* Enumerate all subsets of the prime factors.  For each subset
       we build the unitary divisor d by taking the full prime powers
       of the chosen primes, and we keep track of the exponents of
       the excluded primes.  The excluded primes correspond to the
       factor n/d.  n/d is a perfect power iff the gcd of its
       exponents is ≥ 2 (or if there are no excluded primes, i.e.
       n/d == 1). */
    uint64_t subsets = (cnt >= 64) ? (uint64_t)-1 : (1ULL << cnt);

    for (uint64_t mask = 0; mask < subsets; ++mask) {
        seq_t d = (seq_t)1;
        uint64_t g = 0;          /* gcd of excluded exponents */
        int excl_cnt = 0;        /* number of excluded primes */

        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                /* Include prime i in the divisor: multiply by p^e. */
                d *= pow_uint(primes[i], exps[i]);
            } else {
                /* Exclude prime i: its exponent participates in n/d. */
                ++excl_cnt;
                g = (g == 0) ? exps[i] : gcd_u64(g, exps[i]);
            }
        }

        /* Add d to the sum if n/d is a perfect power. */
        if (excl_cnt == 0 || g >= 2)
            sum += d;
    }

    return sum;
}
