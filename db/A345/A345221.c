/*
 * A345221  Number of divisors of n with an even sum of divisors.
 * https://oeis.org/A345221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} ((sigma(d)+1) mod 2)
 * Range:     u8 n <= 9668, u16 n <= 9668, u32 n <= 9668, u64 n <= 9668,
 *            u128 n <= 9668, big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = number of divisors d of n whose sum of divisors sigma(d) is even.   */

#include <stdint.h>
#include "../utils/isqrt_u64.h"   /* provides isqrt_u64(uint64_t) */

seq_t
a(idx_t n)
{
    /* Factor out powers of 2 from n.  e2 is the exponent of 2. */
    uint64_t m   = (uint64_t)n;
    uint64_t e2  = 0;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }

    /* Products over odd prime exponents.  prod_all = Π (exp+1),  prod_sq = Π (⌊exp/2⌋+1). */
    seq_t prod_all = 1;
    seq_t prod_sq  = 1;

    /* Trial division by odd primes up to sqrt(m). */
    uint64_t limit = isqrt_u64(m);
    for (uint64_t p = 3; p <= limit; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            prod_all *= (seq_t)(exp + 1);
            prod_sq  *= (seq_t)(exp / 2 + 1);
            limit = isqrt_u64(m);   /* recompute if m shrank */
        }
    }

    /* If a prime factor remains. */
    if (m > 1) {                  /* m is a prime with exponent 1 */
        prod_all *= 2;            /* (1+1) */
        /* floor(1/2)+1 = 1, so prod_sq unchanged */
    }

    /* Final result: (e2+1) * (prod_all - prod_sq) */
    seq_t base = (seq_t)(e2 + 1);
    seq_t diff = prod_all - prod_sq;
    return base * diff;
}
