/*
 * A334540  Irregular triangle read by rows: T(n,k) is the number of parts in
 *          the partition of n into k consecutive parts that differ by 5, n >=
 *          1, k >= 1, and the first element of column k is in the row that is
 *          the k-th heptagonal number (A000566).
 * https://oeis.org/A334540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A sequence a(n) for OEIS A334540.
   The irregular triangle T(n,k) is defined by
        T(n,k) = k   if n ≥ Hk and (n – Hk) % k == 0
                 0   otherwise
   where Hk = k*(5*k - 3)/2 (the k‑th heptagonal number).
   a(n) is the n-th entry obtained by reading T(n,k) row‑by‑row,
   including all zeros.  Row r contains columns k = 1 … Kmax(r),
   where Kmax(r) is the largest k with Hk ≤ r.
*/

#include <stdlib.h>
#include <stdint.h>

/* static data used for memoisation */
static idx_t *cumulative = NULL;     /* cumulative[r] = total entries up to row r   */
static size_t    cum_cap   = 0;      /* current capacity of the array              */
static idx_t    cur_rows   = 0;      /* number of rows already computed             */
/* -------------------------------------------------------------- */
/* Heptagonal number Hk = k * (5*k - 3) / 2.                      */
static idx_t heptagonal(idx_t k)
{
    return (k * (5 * k - 3)) / 2;
}

/* Return the maximum k with Hk ≤ n  (used for row width).        */
static idx_t max_k(idx_t n)
{
    idx_t k = 1;
    while (1) {
        idx_t h = heptagonal(k);
        if (h > n)
            return k - 1;
        ++k;
    }
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise the cumulative array if necessary */
    if (!cumulative) {
        cum_cap = 16;
        cumulative = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cumulative) abort();
        cumulative[0] = 0;            /* cumulative[0] = 0 entries before row 1 */
    }

    /* extend the cumulative table until we reach the requested
       index n.  This guarantees that for every index n we know
       the row r such that cumulative[r-1] < n ≤ cumulative[r]. */
    while (cur_rows == 0 || cumulative[cur_rows] < n) {
        /* grow the storage if necessary */
        if (cur_rows >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t *)realloc(cumulative, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            cumulative = tmp;
            cum_cap = new_cap;
        }

        /* compute the next row */
        ++cur_rows;                       /* row number (1‑based) */
        idx_t width = max_k(cur_rows);    /* number of columns in this row */
        cumulative[cur_rows] = cumulative[cur_rows - 1] + width;
    }

    /* binary search to locate the row r where the index n lies */
    idx_t low = 1, high = cur_rows;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cumulative[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;          /* 1‑based row number */
    idx_t col = n - cumulative[row - 1];   /* 1‑based column number */

    /* compute T(row, col) */
    idx_t k = col;
    idx_t h = heptagonal(k);
    if (row >= h && ((row - h) % k) == 0)
        return (seq_t)k;    /* the entry is k */
    else
        return (seq_t)0;    /* the entry is 0 */
}
