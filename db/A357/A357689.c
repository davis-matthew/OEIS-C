/*
 * A357689  a(n) = n/A204455(n), where A204455(n) is the product of odd
 *          noncomposite divisors of n.
 * https://oeis.org/A357689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n/A204455(n) where A204455(n) is the product of odd
 *            noncomposite divisors of n. Multiplicatively, a(p^e)=p^(e-1) if
 *            p>2 and a(2^e)=2^e
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Factor n into distinct primes and exponents. */
    size_t cnt = 0;
    uint64_t primes[64];   /* 64-bit n has < 64 distinct prime factors */
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {                 /* a(2^e) = 2^e */
            for (uint64_t j = 0; j < e; ++j)
                result *= (seq_t)2;
        } else {                      /* a(p^e) = p^(e-1)  (e ≥ 1) */
            /* No multiplication needed for e == 1. */
            for (uint64_t j = 1; j < e; ++j)
                result *= (seq_t)p;
        }
    }

    return result;
}
