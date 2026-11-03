/*
 * A295166  Chromatic invariant of the n-cocktail party graph.
 * https://oeis.org/A295166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*(-1)^(n+k)*(n+k-2)! for n > 1,
 *            a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 98 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    /* Handle the offset term */
    if (n == (idx_t)1)
        return (seq_t)2;

    /* Sum of positive and negative terms separately to avoid
       handling signed arithmetic on seq_t. */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* Current binomial coefficient: binomial(n,k) */
    seq_t bin = 1;

    /* Current factorial value: (n+k-2)! */
    seq_t fact = factorial(n - (idx_t)2);

    for (idx_t k = 0; k <= n; ++k) {
        /* term = bin * fact */
        seq_t term = bin * fact;

        /* sign = (-1)^(n+k)  => negative if (n+k) odd */
        if ((n + k) & (idx_t)1)
            neg_sum += term;
        else
            pos_sum += term;

        if (k == n)
            break;

        /* Update binomial coefficient for next k+1:
           bin = bin * (n - k) / (k + 1)   (exact arithmetic) */
        bin *= (seq_t)(n - k);
        bin /= (seq_t)(k + (idx_t)1);

        /* Update factorial for next k+1:
           fact = fact * (n + k - 1) */
        fact *= (seq_t)(n + k - (idx_t)1);
    }

    /* Result is the absolute difference of the two sums */
    if (pos_sum >= neg_sum)
        return pos_sum - neg_sum;
    else
        return neg_sum - pos_sum;
}
