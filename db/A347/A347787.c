/*
 * A347787  Number of compositions (ordered partitions) of n into at most 6
 *          powers of 2.
 * https://oeis.org/A347787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347787: Number of compositions of n into at most 6 powers of 2.
   Computed via dynamic programming over the number of parts.
   a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 3, ... (see OEIS). */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Allocate two arrays of length n+1 for DP.
       prev[i] = number of compositions of i with exactly the current
       number of parts; curr is used for the next part count. */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !curr) {            /* allocation failure – return 0 */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base case: 0 parts summing to 0.  All other sums are 0. */
    memset(prev, 0, (n + 1) * sizeof(seq_t));
    prev[0] = (seq_t)1;

    /* a(n) is the sum of compositions with 0 to 6 parts. */
    seq_t total = (seq_t)0;
    total += prev[n];   /* contributions from 0 parts (only n==0). */

    /* Iterate over part counts 1 .. 6. */
    for (idx_t parts = 1; parts <= 6; ++parts) {
        /* Zero the current array. */
        memset(curr, 0, (n + 1) * sizeof(seq_t));

        /* For every sum value i, compute compositions with exactly
           `parts` parts: add a composition by appending a power of two
           to each composition that sums to i - p. */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t p = 1;
            while (p <= i) {
                sum += prev[i - p];
                p <<= 1;
            }
            curr[i] = sum;
        }

        /* Add the contribution for sum == n. */
        total += curr[n];

        /* Prepare for next iteration: curr becomes prev, and prev will
           be reused to compute the next level. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Clean up. */
    free(prev);
    free(curr);

    return total;
}
