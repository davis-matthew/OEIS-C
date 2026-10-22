/*
 * A321030  Number of integers x such that 1 <= x <= n and gcd(x,n) =
 *          gcd(x+4,n) = gcd(x+6,n) = gcd(x+10,n) = gcd(x+12,n) = gcd(x+16,n)
 *          = 1.
 * https://oeis.org/A321030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(e-1) * (p > 7 ? (p-6) : 1))
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function defined in the OEIS entry A321030. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle offset and small values */
    if (n <= 1)
        return (seq_t)1;

    /* Factor n */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute p^(e-1) */
        seq_t pow = 1;
        for (uint64_t j = 1; j < e; ++j)
            pow *= (seq_t)p;

        /* Multiply by (p-6) if p > 7 */
        if (p > 7)
            pow *= (seq_t)(p - 6);

        result *= pow;
    }

    return result;
}
