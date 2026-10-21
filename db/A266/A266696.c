/*
 * A266696  a(n) = Sum_{k=3..n} k*StirlingS2(n+1, k+1).
 * https://oeis.org/A266696
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=3..n} k * StirlingS2(n+1, k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the Stirling triangle row by row, keeping only
 * two rows in memory.  For each n it computes the (n+1)‑th row of
 * Stirling numbers of the second kind and accumulates the required sum. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence starts at n = 3; for smaller n we return 0. */
    if (n < 3)
        return 0;

    /* we need the (n+1)-th row: indices 0..n+1.  Allocate two rows. */
    size_t len = (size_t)(n + 2);          /* max index + 1 */
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return 0;   /* allocation failure – return 0 as safe fallback */
    }

    /* Base row S(0,0)=1 */
    prev[0] = 1;

    /* Build rows 1..n+1 */
    for (idx_t i = 1; i <= n + 1; ++i) {
        cur[0] = 0;                     /* S(i,0)=0 for i>0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term1 = prev[k] * (seq_t)k; /* k * S(i-1,k) */
            seq_t term2 = prev[k - 1];       /* S(i-1,k-1)   */
            cur[k] = term1 + term2;
        }
        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* prev now contains S(n+1, k) for k=0..n+1.  Accumulate the sum. */
    seq_t result = 0;
    for (idx_t k = 3; k <= n; ++k) {
        result += (seq_t)k * prev[k + 1];
    }

    /* Clean up */
    free(prev);
    free(cur);

    return result;
}
