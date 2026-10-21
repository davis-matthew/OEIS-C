/*
 * A279929  Expansion of c(q)*c(q^2)/9 - c(q^3)*c(q^6)/3 in powers of q where
 *          c() is a cubic AGM theta function.
 * https://oeis.org/A279929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 0 if 3 | n; otherwise a(n) = ∏_{p^e∥n, p>3} (p^(e+1) –
 *            1)/(p – 1), with a(2^e) = 1. The sequence is multiplicative
 * Range:     u8 n <= 16, u16 n <= 2803, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* offset 1: n ≥ 1 */
    if (n % 3 == 0)
        return 0;          /* any factor 3 forces the value to 0 */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64(n, primes, exps, &count);

    seq_t result = 1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        if (p == 2)
            continue;               /* a(2^e) = 1, contributes nothing */

        /* p > 3 at this point */
        idx_t e = (idx_t)exps[i];

        /* compute p^(e+1) */
        seq_t power = 1;
        for (idx_t j = 0; j < e + 1; ++j)
            power = power * p;

        /* (p^(e+1) – 1)/(p – 1) */
        seq_t mult = (power - 1) / (p - 1);
        result = result * mult;
    }
    return result;
}
