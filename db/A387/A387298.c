/*
 * A387298  Dirichlet g.f.: zeta(s) * zeta(6*s) / (zeta(3*s) * zeta(4*s)).
 * https://oeis.org/A387298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387298: multiplicative function with a(p^e) = ((e^2) mod 3) * (-1)^floor((e-1)/3) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* Maximum number of distinct prime factors of a 64‑bit integer is < 20, 64 is safe. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* factor_u64 fills pr and ex with the distinct prime factors of n and their
       exponents; cnt is set to the number of factors. */
    factor_u64(n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        /* Exponent of current prime. */
        idx_t e = (idx_t)ex[i];

        /* If e ≡ 0 (mod 3), the value for this prime power is 0. */
        if (e % 3 == 0)
            return 0;

        /* Sign: (-1)^floor((e-1)/3). */
        idx_t s = (e - 1) / 3;
        if (s & 1U)
            result = -result;
    }

    return result;
}
