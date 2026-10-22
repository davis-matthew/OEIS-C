/*
 * A382182  Lexicographically earliest increasing sequence starting with a(0)
 *          = 1 such that the polynomial which interpolates the first k values
 *          has degree k-1 and only integer coefficients.
 * https://oeis.org/A382182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n+1} binomial(n, m-1)*(m-1)!*s(m), where s(m) =
 *            -1 if m > 3 && m % 3 == 2, otherwise s(m) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;   /* sum of terms with positive sign */
    seq_t sum_neg = 0;   /* sum of terms with negative sign */
    seq_t binom   = 1;   /* binomial(n, 0) */
    seq_t fact    = 1;   /* (m-1)! for current m */

    for (idx_t m = 1; m <= n + 1; ++m) {
        seq_t term = binom * fact;  /* binomial(n, m-1) * (m-1)! */

        int negative = (m > 3 && (m % 3) == 2);
        if (negative)
            sum_neg += term;
        else
            sum_pos += term;

        /* Update binomial coefficient for next k (if not the last iteration) */
        if (m < n + 1) {
            /* binom_{k+1} = binom_k * (n - k) / (k + 1), with k = m-1 */
            seq_t tmp = binom * (seq_t)(n - (m - 1));
            binom = tmp / (seq_t)m;
        }

        /* Update factorial for next m */
        fact *= (seq_t)m;
    }

    return sum_pos - sum_neg;
}
