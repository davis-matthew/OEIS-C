/*
 * A284458  Number of pairs (f,g) of endofunctions on [n] such that the
 *          composite function gf has no fixed point.
 * https://oeis.org/A284458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k)^2 * k! * n^(2*(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 209 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t sn = (seq_t)n;

    /* Square of n for dividing the power term */
    seq_t n_sq = sn * sn;

    /* Initial power term n^(2*n) */
    int exp = (int)(2 * n);
    seq_t pow_n = ipow_i32(sn, exp);

    seq_t binom = 1; /* C(n,0) */
    seq_t fact  = 1; /* 0! */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = n^(2*(n-k)) * k! * C(n,k)^2 */
        seq_t term = pow_n * fact;
        term *= binom;
        term *= binom;

        if (k & 1)
            result -= term;
        else
            result += term;

        if (k == n) break;

        /* Update binomial coefficient for next k */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update factorial for next k */
        fact *= (seq_t)(k + 1);

        /* Update power term: divide by n^2 */
        pow_n /= n_sq;
    }

    return result;
}
