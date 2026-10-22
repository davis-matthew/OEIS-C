/*
 * A391183  a(n) = 1 if n is a number of the form x^2 + 2*y^2, and otherwise
 *          0.
 * https://oeis.org/A391183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 if n can be written as x^2 + 2*y^2, otherwise 0.
 * This follows from the multiplicative property: a(p^e)=0 if 
 * p ≡ 5 or 7 (mod 8) and e is odd; otherwise a(p^e)=1.
 * 
 * We factor n using the provided helper function factor_u64
 * and test the prime factors against the above rule.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* 0 = 0^2 + 2*0^2 */

    /* n fits in uint64_t, so we can factor it with the helper. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if ((p & 7ULL) == 5 || (p & 7ULL) == 7) {
            if (e & 1ULL)          /* odd exponent of forbidden prime */
                return (seq_t)0;
        }
    }
    return (seq_t)1;
}
