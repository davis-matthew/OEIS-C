/*
 * A351314  Sum of the 8th powers of the square divisors of n.
 * https://oeis.org/A351314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2|n} (d^2)^8
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative form is used, avoiding huge intermediate powers. */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety - OEIS offset starts at 1 */

    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* prime with exponent < 2 contributes 1 */
        if (e < 2) continue;

        uint64_t k = e / 2;           /* floor(e/2) */

        /* compute p^16, guaranteed to fit in seq_t for all outputs that fit in seq_t */
        seq_t pow_p16 = 1;
        for (idx_t j = 0; j < 16; ++j)
            pow_p16 *= p;

        /* sum_{i=0}^{k} (p^16)^i  = 1 + p^16 + p^32 + ... + p^(16*k) */
        seq_t cur = 1;
        seq_t sum = 1;
        for (uint64_t t = 1; t <= k; ++t)
        {
            cur *= pow_p16;
            sum += cur;
        }

        result *= sum;
    }

    return result;
}
