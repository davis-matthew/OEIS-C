/*
 * A285867  Triangle T(n, k) read by rows: T(n, k) = S2(n, k)*k! + S2(n,
 *          k-1)*(k-1)! with the Stirling2 triangle S2 = A048993.
 * https://oeis.org/A285867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n, k) read by rows:  
   a(n) = T(row, col) where row = the unique r with r(r+1)/2 ≤ n < (r+1)(r+2)/2,  
   col = n - r(r+1)/2.  
   T(n, k) = S₂(n, k)·k! + S₂(n, k-1)·(k-1)!  (Stirling numbers of the second kind).  
   Recurrence (for n > k ≥ 0):  
      T(n, k) = (k-1)*T(n-1, k-1) + k*T(n-1, k).  
   Boundary: T(0,0)=1, T(n,0)=0 for n>0, T(n,n) = (n+1)!/2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r that contains the index n.  Use a small loop
       (r never exceeds ~141 for n ≤ 10000). */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* Row r is found.  Compute the column k. */
    idx_t prev_tri = r * (r + 1) / 2;          /* sum of lengths of rows 0..r-1 */
    idx_t k = n - prev_tri;                   /* 0 ≤ k ≤ r */

    /* Special case: row 0 contains only the element 1. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate two rows of size r+1. */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!prev || !curr) {                     /* allocation failure – unlikely */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base row (row 0). */
    prev[0] = (seq_t)1;

    /* Variable to hold (i+1)! as we go along.  Start with 1 (0! = 1). */
    seq_t factorial = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        /* First element of each row (except row 0) is 0. */
        curr[0] = (seq_t)0;

        /* Off–diagonal elements for 1 ≤ j < i.  Use 128‑bit intermediates
           to avoid overflow when seq_t has more than 64 bits. */
        for (idx_t j = 1; j < i; ++j) {
            seq_t term1 = (seq_t)((unsigned __int128)(j - 1) * prev[j - 1]);
            seq_t term2 = (seq_t)((unsigned __int128)j * prev[j]);
            curr[j] = term1 + term2;
        }

        /* Diagonal element T(i,i) = (i+1)! / 2. */
        factorial = (seq_t)((unsigned __int128)factorial * (i + 1));
        curr[i] = factorial / ((seq_t)2);

        /* Prepare for next iteration. */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
