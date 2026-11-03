/*
 * A341597  a(n) = A144066(n+3,3).
 * https://oeis.org/A341597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+6,6) + 53*binom(n+5,6) + 91*binom(n+4,6) +
 *            15*binom(n+3,6) (coefficients come from (15*x^3 + 91*x^2 + 53*x
 *            + 1)/(1 – x)^7)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t result = binomial(n + 6, 6);

    /* term for 53 * C(n+5, 6) */
    seq_t t = mul_small(binomial(n + 5, 6), 53u);
    result += t;

    /* term for 91 * C(n+4, 6) */
    t = mul_small(binomial(n + 4, 6), 91u);
    result += t;

    /* term for 15 * C(n+3, 6) */
    t = mul_small(binomial(n + 3, 6), 15u);
    result += t;

    return result;
}
