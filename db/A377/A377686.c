/*
 * A377686  E.g.f. satisfies A(x) = (1 - x * log(1 - x*A(x)))^3.
 * https://oeis.org/A377686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..⌊n/2⌋} 3 * (n!/(n-k)!) * C(n-k, k) *
 *            ∏_{i=3n-4k+4}^{3n-3k+2} i (unsigned Stirling first kind) a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: C(m,k) is the unsigned Stirling number of the first kind.  */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------------
   Compute the unsigned Stirling numbers of the first kind up to the needed
   arguments by a single forward DP pass.  While iterating through the rows,
   we capture the values required for the final sum (C(n-k, k) for all
   k=1..⌊n/2⌋) and store them in the array `res`.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    idx_t kmax = n / 2;                   /* floor(n/2) */

    /* Allocate arrays for current and previous rows of Stirling numbers. */
    seq_t *prev = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
    seq_t *res  = (seq_t *)calloc(kmax + 1, sizeof(seq_t)); /* C(n-k,k) */

    prev[0] = (seq_t)1;   /* c(0,0)=1 */

    /* Forward DP to compute all required C(n-k,k). */
    for (idx_t m = 1; m <= n; ++m) {
        cur[0] = (seq_t)0;                           /* c(m,0)=0 for m>0 */
        idx_t maxk = (m < kmax) ? m : kmax;
        for (idx_t k = 1; k <= maxk; ++k) {
            /* cur[k] = (m-1) * prev[k] + prev[k-1]; */
            cur[k] = prev[k] * (seq_t)(m - 1) + prev[k - 1];
        }
        /* Capture C(n-k,k) when the current row m == n-k. */
        idx_t k_needed = n - m;
        if (k_needed >= 1 && k_needed <= kmax) {
            res[k_needed] = cur[k_needed];
        }
        /* Swap prev and cur for next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* --------------------------------------------------------------------
       Sum the terms of the sequence.
       -------------------------------------------------------------------- */
    seq_t total = (seq_t)0;
    seq_t prod_factor = (seq_t)1;                 /* n!/(n-k)! */

    for (idx_t k = 1; k <= kmax; ++k) {
        /* Update product n!/(n-k)! incrementally. */
        prod_factor *= (seq_t)(n - k + 1);

        /* Compute product over range [3n-4k+4, 3n-3k+2]. */
        seq_t p = (seq_t)1;
        idx_t start = 3 * n - 4 * k + 4;
        idx_t end   = 3 * n - 3 * k + 2;
        if (start <= end) {
            for (idx_t t = start; t <= end; ++t) {
                p *= (seq_t)t;
            }
        }

        /* term = 3 * prod_factor * res[k] * p. */
        seq_t term = prod_factor * res[k];
        term = term * p;
        term = term * (seq_t)3;

        total += term;
    }

    /* Free temporary arrays. */
    free(prev);
    free(cur);
    free(res);

    return total;
}
