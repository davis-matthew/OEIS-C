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
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

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
