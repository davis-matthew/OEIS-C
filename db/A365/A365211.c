/*
 * A365211  The sum of divisors d of n such that gcd(d, n/d) is a 5-rough
 *          number (A007310).
 * https://oeis.org/A365211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = 1 + p^e for p = 2 and 3, a(p^e) =
 *            (p^(e+1)-1)/(p-1) for primes p ≥ 5
 * Range:     u8 n <= 16, u16 n <= 1723, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* helper: power of unsigned integer in seq_t, exponent is idx_t */
static seq_t seq_pow_uint64(uint64_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe */
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = exps[i];
        seq_t term;
        if (p == 2ULL || p == 3ULL) {
            seq_t pe = seq_pow_uint64(p, e);
            term = pe + 1;
        } else {
            seq_t pe1 = seq_pow_uint64(p, e + 1);   /* p^(e+1) */
            term = (pe1 - 1) / ((seq_t)p - 1);
        }
        result *= term;
    }
    return result;
}
