/*
 * A243633  Expansion of (4*x^3-6*x^2+4*x-1)/(5*x^3-8*x^2+5*x-1).
 * https://oeis.org/A243633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=0..n} Sum_{k=0..⌊n/2⌋} binomial(m+k-1,k) *
 *            binomial(n+m-1,n-4*k-m)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the double sum directly using incremental updates to avoid expensive
   binomial computations for every term. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* Provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n) {
    /* Special case: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t result = 0;

    /* Loop over m from 1 to n.  The terms with m==0 are zero when n>0. */
    for (idx_t m = 1; m <= n; ++m) {
        /* Compute constant terms for this m */
        idx_t N = n + m - 1;        /* n+m-1 */
        idx_t sub0 = n - m;         /* n-4*0-m = n-m (>=0) */

        /* Initial value of C(n+m-1, sub0) */
        seq_t binom2 = binomial(N, sub0);

        /* C(m+0-1,0) = 1 for any m>0 */
        seq_t cur1 = 1;

        /* Add term for k = 0 */
        result += cur1 * binom2;

        /* Maximum k such that 4*k + m <= n */
        idx_t maxk = (n - m) / 4;
        if (maxk == 0)
            continue;

        /* Temporary values for incremental updates */
        idx_t t_prev = sub0;   /* sub for previous k */

        for (idx_t k = 1; k <= maxk; ++k) {
            /* Update cur1 = C(m+k-1, k) from previous value */
            cur1 = (cur1 * (seq_t)(m + k - 1)) / (seq_t)k;

            /* Update binom2 from C(N, t_prev) to C(N, t_prev-4) */
            /* Compute numerator = (t_prev-3)*(t_prev-2)*(t_prev-1)*t_prev */
            seq_t num = (seq_t)(t_prev - 3) *
                        (seq_t)(t_prev - 2) *
                        (seq_t)(t_prev - 1) *
                        (seq_t)t_prev;
            /* Compute denominator = (N-t_prev+1)*(N-t_prev+2)*(N-t_prev+3)*(N-t_prev+4) */
            seq_t den = (seq_t)(N - t_prev + 1) *
                        (seq_t)(N - t_prev + 2) *
                        (seq_t)(N - t_prev + 3) *
                        (seq_t)(N - t_prev + 4);

            binom2 = (binom2 * num) / den;

            /* Add contribution for this k */
            result += cur1 * binom2;

            /* Prepare for next iteration */
            t_prev -= 4;
        }
    }

    return result;
}
