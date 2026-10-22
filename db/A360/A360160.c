/*
 * A360160  a(n) is the sum of unitary divisors of n that are odd squares.
 * https://oeis.org/A360160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏{p^e∥n, p>2, e even}(1 + p^e); (p=2 or e odd contribute
 *            a factor 1)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n == 0) return 0;          /* undefined for n<1, just guard */
    if (n == 1) return 1;

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];           /* enough to hold all prime factors of a 64‑bit integer */
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];      /* exponent is small, fits in an int */

        /* ignore the prime 2 – odd squares contain no factor 2          */
        if (p == 2) continue;

        /* if exponent is even, include the term 1 + p^e, otherwise skip */
        if ((e & 1) == 0) {
            seq_t pe = 1;
            for (int j = 0; j < e; ++j)
                pe *= (seq_t)p;      /* compute p^e in seq_t */

            result *= (pe + 1);     /* multiply by the unitary divisor sum contribution */
        }
    }

    return result;
}
