/*
 * A371300  Triangle read by rows: Riordan array (1/(1 - x), (1 + x)/(1 - x -
 *          x^2)).
 * https://oeis.org/A371300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 2*T(n-1,k-1) + T(n-1,k) + T(n-2,k-1) + T(n-2,k), with
 *            T(n,0)=1 and T(n,k)=0 for k<0 or k>n. a(n) = T(r,k) where
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* 0‑based column index */

    /* Trivial case: first row only contains T(0,0)=1. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate rows: prev2 = T(m-2,*), prev1 = T(m-1,*) */
    seq_t *prev2 = NULL;
    seq_t *prev1 = NULL;

    /* Row 0: T(0,0) = 1 */
    prev1 = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!prev1) return 0;          /* out of memory → return 0 */
    prev1[0] = (seq_t)1;

    /* Build rows 1 … r */
    for (idx_t m = 1; m <= r; ++m) {
        size_t cur_len = (size_t)(m + 1);
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * cur_len);
        if (!cur) {                 /* cleanup and abort on failure */
            free(prev2);
            free(prev1);
            return 0;
        }

        /* T(m,0) = 1 */
        cur[0] = (seq_t)1;

        /* Compute other entries using the recurrence */
        for (idx_t q = 1; q <= m; ++q) {
            seq_t val = 0;

            /* 2*T(m-1,q-1) – always present */
            val += (seq_t)2 * prev1[q - 1];

            /* + T(m-1,q) – only if q ≤ m-1 */
            if (q < m) val += prev1[q];

            /* + T(m-2,q-1) – only if prev2 exists and q ≤ m-1 */
            if (prev2 && q < m) val += prev2[q - 1];

            /* + T(m-2,q) – only if prev2 exists and q ≤ m-2 */
            if (prev2 && q < m - 1) val += prev2[q];

            cur[q] = val;
        }

        /* Advance: prev2 ← prev1, prev1 ← cur, free old prev2 */
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    /* Now prev1 holds row r.  The desired element is at column k. */
    seq_t result = (k < (size_t)(r + 1)) ? prev1[k] : (seq_t)0;

    /* Clean up */
    free(prev2);
    free(prev1);

    return result;
}
