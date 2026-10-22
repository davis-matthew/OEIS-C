/*
 * A400142  The number of divisors d of n such that gcid(d, n/d) is odd, where
 *          gcid is the greatest common infinitary divisor.
 * https://oeis.org/A400142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} f(p^e) where f(p^e) = e+1 for odd prime p
 *            2^{popcount(e)} for p = 2 (Multiplicative with a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* compute popcount of a 64‑bit integer without compiler builtin */
static unsigned int popcount_uint64(uint64_t x) {
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for OEIS, but defensive */
    uint64_t primes[20], exps[20];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            unsigned int ones = popcount_uint64(exps[i]);
            seq_t val = 1;
            while (ones--) val <<= 1;   /* 2^ones */
            result *= val;
        } else {
            result *= (seq_t)(exps[i] + 1);
        }
    }
    return result;
}
