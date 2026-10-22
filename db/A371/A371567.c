/*
 * A371567  Array read by downward antidiagonals: A(n,k) = A(n-1,k+1) +
 *          (k+1)*Sum_{j=0..k} A(n-1,j) with A(0,k) = k+1, n >= 0, k >= 0.
 * https://oeis.org/A371567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by downward antidiagonals of the array A defined by
 *            A(0,k) = k+1 A(n,k) = A(n-1,k+1) + (k+1) * sum_{j=0..k} A(n-1,j)
 *            for n>=1,k>=0 The sequence is OEIS A371567
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Determine diagonal number d and position i on that diagonal.
       The diagonal d satisfies d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    while (((unsigned __int128)(d + 1) * (d + 2) / 2) <= n) d++;
    while (((unsigned __int128)d * (d + 1) / 2) > n) d--;
    idx_t i = n - (idx_t)((unsigned __int128)d * (d + 1) / 2);
    idx_t j = d - i;

    /* Allocate temporary rows of size up to d+3.  They are reused for each
       call, resizing only when a larger diagonal is needed. */
    static seq_t *prev = NULL, *curr = NULL;
    static size_t prev_cap = 0;
    size_t needed = (size_t)d + 3;
    if (prev_cap < needed) {
        free(prev);
        free(curr);
        prev = (seq_t *)malloc(needed * sizeof(seq_t));
        curr = (seq_t *)malloc(needed * sizeof(seq_t));
        prev_cap = needed;
    }

    /* Initialise row 0 (n = 0) up to index d+1. */
    for (size_t k = 0; k <= (size_t)d + 1; ++k) {
        prev[k] = (seq_t)(k + 1);
    }

    /* Build rows 1 .. i.  After completing row i, prev holds A(i,k) for
       k = 0..d. */
    for (idx_t m = 1; m <= i; ++m) {
        seq_t cum = 0;
        for (size_t k = 0; k <= (size_t)d; ++k) {
            cum += prev[k];
            curr[k] = prev[k + 1] + (seq_t)(k + 1) * cum;
        }
        /* Copy new row into prev for next iteration. */
        for (size_t k = 0; k <= (size_t)d; ++k) {
            prev[k] = curr[k];
        }
    }

    /* The desired value is A(i,j). */
    return prev[j];
}
