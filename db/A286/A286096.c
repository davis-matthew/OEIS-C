/*
 * A286096  Triangle read by rows giving numerators of the Fourier expansion
 *          of cos^n(x).
 * https://oeis.org/A286096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A286096: numerators of the Fourier expansion of cos^n(x).
 * The triangle T(n,k) is defined by
 *     T(0,0) = 1
 *     T(n,0) = T(n-1,1)
 *     T(n,1) = 2*T(n-1,0) + T(n-1,2)
 *     T(n,k) = T(n-1,k-1) + T(n-1,k+1)          for 2 <= k <= n-1
 *     T(n,n) = T(n-1,n-1)
 * Only values with 0 <= k <= n are non‑zero.
 * 
 * The flattened sequence is read row by row (offset 0): 
 *     a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 * 
 * The function below recomputes the required row of the triangle
 * using dynamic programming and returns the requested term.
 */

#include <stdlib.h>

/* compute integer triangular number r*(r+1)/2 safely */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* locate the row number r such that triangular(r) <= n < triangular(r+1) */
static inline idx_t locate_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponential search for an upper bound */
    while (triangular(high) <= n) high <<= 1;
    /* binary search in [high/2, high) */
    low = high >> 1;
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* special case: first term */
    if (n == 0) return (seq_t)1;

    /* find row r and column c (0‑based) */
    idx_t r = locate_row(n);
    idx_t start = triangular(r);
    idx_t c   = n - start;

    /* allocate two rows of size r+1 */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev || !curr) return (seq_t)0;  /* out of memory fallback */

    prev[0] = (seq_t)1;                     /* T(0,0) = 1 */

    /* DP from row 1 up to row r */
    for (idx_t m = 1; m <= r; ++m)
    {
        /* k = 0 */
        curr[0] = (m - 1 >= 1 ? prev[1] : (seq_t)0);

        if (m >= 1)
        {
            /* k = 1 (special case) */
            seq_t a_val = prev[0];
            seq_t b_val = (m - 1 >= 2 ? prev[2] : (seq_t)0);
            curr[1] = a_val + a_val + b_val;

            /* middle entries 2 .. m-1 */
            for (idx_t k = 2; k < m; ++k)
            {
                seq_t left  = prev[k - 1];
                seq_t right = (k + 1 <= m - 1 ? prev[k + 1] : (seq_t)0);
                curr[k] = left + right;
            }

            /* k = m (last entry) */
            curr[m] = prev[m - 1];
        }

        /* swap previous and current rows for next iteration */
        seq_t *tmp  = prev;
        prev       = curr;
        curr       = tmp;
    }

    /* result is prev[c] after the last iteration (row r) */
    seq_t result = prev[c];

    free(prev);
    free(curr);
    return result;
}
