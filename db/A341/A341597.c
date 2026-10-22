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
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"      /* binom(idx_t, idx_t) */
#include "../utils/mul_small.h"  /* mul_small(seq_t, unsigned) */

seq_t a(idx_t n)
{
    seq_t result = binom(n + 6, 6);

    /* term for 53 * C(n+5, 6) */
    seq_t t = mul_small(binom(n + 5, 6), 53u);
    result += t;

    /* term for 91 * C(n+4, 6) */
    t = mul_small(binom(n + 4, 6), 91u);
    result += t;

    /* term for 15 * C(n+3, 6) */
    t = mul_small(binom(n + 3, 6), 15u);
    result += t;

    return result;
}
