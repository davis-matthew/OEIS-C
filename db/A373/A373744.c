/*
 * A373744  Triangle read by rows: the almost-Riordan array ( 1/(1-x) |
 *          2/((1-x)*(1+sqrt(1-4*x))), (1-2*x-sqrt(1-4*x))/(2*x) ).
 * https://oeis.org/A373744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows of the almost‑Riordan array A373744.
   a(n) is the (n‑th) entry of the triangle:
      T(0,0)=1,
      T(n,0)=T(n,n)=1,
      T(n,k)=T(n-1,k-1)+(2-δ_{k,1})*T(n-1,k)+T(n-1,k+1)   (1≤k≤n−1).
   The index n runs row by row starting at 0. */

#include <stdlib.h>

/* Locate the row r containing index n:
   r is the greatest integer with r(r+1)/2 ≤ n.     */
static inline idx_t row_of(idx_t n)
{
    idx_t low  = 0;
    idx_t high = (idx_t)0xFFFFFFFFULL;   /* enough for n up to 2^64-1 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*(mid+1)/2 may overflow 64 bits; use 128‑bit */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        idx_t sum = (idx_t)(prod / 2);
        if (sum <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;          /* last mid satisfying sum ≤ n */
}

seq_t a(idx_t n)
{
    /* static storage for produced rows */
    static seq_t **rows   = NULL;   /* array of pointers to rows */
    static size_t  rows_cnt = 0;    /* number of rows stored (0 … rows_cnt-1) */

    /* Find which row contains the requested index. */
    idx_t r = row_of(n);
    idx_t offset = n - (idx_t)(( (unsigned __int128)r * (r + 1) ) / 2);

    /* Grow the cache enough to contain row r. */
    while (rows_cnt <= (size_t)r) {
        size_t rr = rows_cnt;          /* next row index to create */
        seq_t *newrow = NULL;

        if (rr == 0) {                  /* first row : [1] */
            newrow = (seq_t *)malloc(sizeof(seq_t));
            if (!newrow) abort();
            newrow[0] = 1;
        } else if (rr == 1) {          /* second row : [1,1] */
            newrow = (seq_t *)malloc(2 * sizeof(seq_t));
            if (!newrow) abort();
            newrow[0] = 1;
            newrow[1] = 1;
        } else {
            seq_t *prev = rows[rr - 1];
            size_t len = rr + 1;           /* length of the new row */
            newrow = (seq_t *)calloc(len, sizeof(seq_t));
            if (!newrow) abort();

            newrow[0] = 1;
            newrow[rr] = 1;
            for (size_t k = 1; k < rr; ++k) {
                seq_t left  = prev[k - 1];
                seq_t mid   = prev[k];
                seq_t right = (k + 1 < rr) ? prev[k + 1] : 0;
                seq_t coef  = (k == 1) ? 1 : 2;
                newrow[k] = left + coef * mid + right;
            }
        }

        /* Add the new row pointer to the table. */
        seq_t **rows_new = (seq_t **)realloc(rows, (rows_cnt + 1) * sizeof(seq_t *));
        if (!rows_new) abort();
        rows = rows_new;
        rows[rows_cnt] = newrow;
        ++rows_cnt;
    }

    return rows[(size_t)r][(size_t)offset];
}
