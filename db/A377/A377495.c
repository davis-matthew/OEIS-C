/*
 * A377495  E.g.f. satisfies A(x) = 1/(1 - A(x)^2 * (exp(x*A(x)^2) - 1)).
 * https://oeis.org/A377495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*n+3*k)!/(2*n+2*k+1)! * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a direct summation of the formula.  It
 * builds Stirling numbers of the second kind recursively while
 * staying within O(n²) arithmetic operations per call.  The
 * intermediate products are computed multiplicatively so that
 * intermediate values never exceed what is necessary for the
 * result.  The function works for any `seq_t` type supplied by the
 * build: unsigned integers or an arbitrary‑precision integer class. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate two rows for Stirling numbers (S(n,k)).  Size n+1
     * because we need indices up to k = n. */
    seq_t *prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev || !cur) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Base row S(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Accumulator for a(n) */
    seq_t sum = (seq_t)0;

    /* Iterate over rows 1 .. n */
    for (idx_t row = (idx_t)1; row <= n; ++row) {
        cur[0] = (seq_t)0;           /* S(row,0) = 0 for row > 0 */

        /* Compute Stirling numbers for this row and accumulate the
           summation term where required. */
        for (idx_t k = (idx_t)1; k <= row; ++k) {
            /* S(row,k) = k * S(row-1,k) + S(row-1,k-1) */
            cur[k] = prev[k] * (seq_t)k + prev[k - 1];

            /* Compute the factorial ratio product:
               prod = Π_{t = 2*row + 2*k + 2}^{2*row + 3*k} t
               The loop runs for k-1 iterations, which are zero when k = 1. */
            seq_t prod = (seq_t)1;
            if (k > (idx_t)1) {
                idx_t start = (idx_t)(2 * row + 2 * k + 2);
                idx_t end   = (idx_t)(2 * row + 3 * k);
                for (idx_t t = start; t <= end; ++t) {
                    prod *= (seq_t)t;
                }
            }

            /* Add contribution to the sum when we are at the final row. */
            if (row == n) {
                sum += prod * cur[k];
            }
        }

        /* Prepare for next iteration: swap previous and current rows. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* The final row data is now in 'prev', but we no longer need it. */
    free(prev);
    free(cur);

    return sum;
}
