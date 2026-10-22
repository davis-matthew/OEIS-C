/*
 * A372671  a(n) = phi(6 * n)/2.
 * https://oeis.org/A372671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(6 * n)/2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is multiplicative with a(p^e) = p^e for p=2 or 3,
   and a(p^e) = (p-1)*p^(e-1) for other primes. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but return 0 safely */

    /* Factor n into prime powers.  We assume factor_u64 fills at most 64 distinct primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];

        /* Compute p^exp as seq_t */
        seq_t ppow = 1;
        for (uint64_t k = 0; k < exp; ++k) ppow *= (seq_t)p;

        if (p == 2 || p == 3) {
            /* For p = 2 or 3: a(p^e) = p^e */
            result *= ppow;
        } else {
            /* For other primes: a(p^e) = (p-1)*p^(e-1) */
            seq_t term = (seq_t)(p - 1);
            for (uint64_t k = 1; k < exp; ++k) term *= (seq_t)p;
            result *= term;
        }
    }

    return result;
}
