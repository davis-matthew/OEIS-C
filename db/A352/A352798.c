/*
 * A352798  a(n) = 1/(cf[0;n,n,n,...,n] - cf[0;n,n,...,n]) where the first
 *          continued fraction has n+1 terms and the second has n terms.
 * https://oeis.org/A352798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1/(cf[0;n,n,...,n] – cf[0;n,n,...,n]) where the first
 *            continued * fraction has n terms and the second has n‑1 terms.
 *            The implementation * uses the recurrence for the numerators p_k
 *            and denominators q_k of the * convergents of the continued
 *            fraction with all partial quotients equal to * n:...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* p_prev2/q_prev2 = S_0 = 0/1   (base case for k = 0) */
    seq_t p_prev2 = 0;
    seq_t q_prev2 = 1;

    /* p_prev1/q_prev1 = S_1 = 1/n   (base case for k = 1) */
    seq_t p_prev1 = 1;
    seq_t q_prev1 = (seq_t)n;

    /* Special case n == 1 */
    if (n == 1) {
        /* a(1) = 1/(1/1 - 0) = 1 */
        return 1;
    }

    /* Iteratively compute up to S_n.  After the loop,
       p_prev1 = p_n, q_prev1 = q_n,
       p_prev2 = p_{n-1}, q_prev2 = q_{n-1}. */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t p = (seq_t)n * p_prev1 + p_prev2;
        seq_t q = (seq_t)n * q_prev1 + q_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    seq_t numerator_diff = p_prev1 * q_prev2 - p_prev2 * q_prev1;
    seq_t denom          = q_prev1 * q_prev2;

    /* numerator_diff is always ±1; division yields the desired integer. */
    return denom / numerator_diff;
}
