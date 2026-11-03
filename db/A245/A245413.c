/*
 * A245413  Number of endofunctions on [n] such that no element has a preimage
 *          of cardinality nine.
 * https://oeis.org/A245413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] (exp(x)-x^9/9!)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 190 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

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
    /* handle n = 0 by definition */
    if (n == 0U) return (seq_t)1;

    /* 9! = 362880 */
    const seq_t NINE_F = 362880U;

    /* constants used for the iterative computation */
    const idx_t maxK = n / 9U;

    seq_t result   = 0;
    seq_t falling  = 1;          /* falling = n! / (n-9*k)!   – initially k = 0 */
    seq_t denom9   = 1;          /* (9!)^k                   – initially k = 0 */

    for (idx_t k = 0; k <= maxK; ++k) {
        /* binomial coefficient C(n, k) */
        seq_t bin  = binomial(n, k);

        /* base and exponent for (n-k)^(n-9k) */
        seq_t base = (seq_t)(n - k);
        idx_t  exp = n - 9 * k;

        seq_t pow = ipow(base, (seq_t)exp);

        /* term = C(n,k) * falling * pow / denom9 */
        seq_t term = bin;
        term *= falling;
        term *= pow;
        term /= denom9;  /* exact division */

        /* accumulate with alternating sign */
        if (k & 1U) {
            result -= term;
        } else {
            result += term;
        }

        /* update falling and denom9 for the next k
           (preparation for k+1) */
        if (k == maxK) break;

        /* multiply falling by the next block of 9 descending factors */
        for (int t = 0; t < 9; ++t) {
            falling *= (seq_t)(n - 9 * k - t);
        }

        /* multiply denom9 by 9! for the next k */
        denom9 *= NINE_F;
    }

    return result;
}
