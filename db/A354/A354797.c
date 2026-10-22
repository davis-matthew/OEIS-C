/*
 * A354797  Triangle read by rows. T(n, k) = |Stirling1(n, k)| * Stirling2(n +
 *          k, n) = A132393(n, k) * A048993(n + k, n).
 * https://oeis.org/A354797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = |Stirling1(row,row_col)| * Stirling2(row+row_col,row)
 *            (row, row_col) are the zero‑based coordinates of the n-th
 *            element when the triangle T(n,k)=|S1(n,k)|*S2(n+k,n) is read row
 *            by row. T(0,0)=1, T(1,0)=0, T(1,1)=1, T(2,0)=0, T(2,1)=3,
 *            T(2,2)=7, …
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 41, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   ensure_pointer_capacity, find_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/ensure_pointer_capacity.h"
#include "../utils/find_col.h"

/* ------------------------------------------------------------------ */
/*  Stirling numbers (unsigned)                                       */
/* ------------------------------------------------------------------ */

/* temporary storage for rows of Stirling numbers of first kind */
static seq_t **st1_rows = NULL;
static idx_t   st1_len  = 0;   /* number of rows already constructed */
static size_t  st1_cap  = 0;   /* capacity of st1_rows pointer array */

/* temporary storage for rows of Stirling numbers of second kind */
static seq_t **st2_rows = NULL;
static idx_t   st2_len  = 0;
static size_t  st2_cap  = 0;

/* build rows of Stirling first kind up to and including n */
static void ensure_stirling1(idx_t upto)
{
    if (st1_len > upto) return;
    ensure_pointer_capacity(&st1_cap, &st1_rows, upto + 1);
    for (idx_t n = st1_len; n <= upto; ++n) {
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (n == 0) {                 /* S1(0,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            row[0] = (seq_t)0;        /* S1(n,0) = 0 for n>0 */
            for (idx_t k = 1; k <= n; ++k) {
                seq_t part1 = (k <= n - 1) ? st1_rows[n - 1][k] : (seq_t)0;
                seq_t part2 = st1_rows[n - 1][k - 1];
                row[k] = part2 + (seq_t)(n - 1) * part1;
            }
        }
        st1_rows[n] = row;
        ++st1_len;
    }
}

/* build rows of Stirling second kind up to and including n */
static void ensure_stirling2(idx_t upto)
{
    if (st2_len > upto) return;
    ensure_pointer_capacity(&st2_cap, &st2_rows, upto + 1);
    for (idx_t n = st2_len; n <= upto; ++n) {
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (n == 0) {                 /* S2(0,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            row[0] = (seq_t)0;        /* S2(n,0) = 0 for n>0 */
            for (idx_t k = 1; k <= n; ++k) {
                seq_t part1 = (k <= n - 1) ? (seq_t)k * st2_rows[n - 1][k] : (seq_t)0;
                seq_t part2 = st2_rows[n - 1][k - 1];
                row[k] = part2 + part1;
            }
        }
        st2_rows[n] = row;
        ++st2_len;
    }
}

/* ------------------------------------------------------------------ */
/*  Convert flat index to (row, column)                               */
/* ------------------------------------------------------------------ */
static inline idx_t find_row(idx_t idx)
{
    /* solve r(r+1)/2 <= idx < (r+1)(r+2)/2  */
    double d = sqrt((double)(8ULL * idx + 1));
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    while ((r + 1) * (r + 2) / 2 <= idx) ++r;
    while (r * (r + 1) / 2 > idx) --r;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Public API                                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t col = find_col(n, row);
    ensure_stirling1(row);
    /* we need S2(row+col, row) */
    ensure_stirling2(row + col);
    seq_t s1 = st1_rows[row][col];
    seq_t s2 = st2_rows[row + col][row];
    return s1 * s2;
}
