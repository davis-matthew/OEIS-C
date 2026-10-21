/*
 * A247051  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,2) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 78, u32 n <= 147, u64 n <= 285,
 *            u128 n <= 564, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247051
   a(n) = T(n/3 , n%3) where T(k,0)=T(k-1,1),
                               T(k,1)=T(k-1,0)+T(k-1,2),
                               T(k,2)=T(k-1,1)+T(k-1,0).
   Initial column k=0: T(0,0)=T(0,1)=0 , T(0,2)=1. */
#include <stddef.h>
#include <stdlib.h>

static seq_t *col_vals = NULL;        /* flattened 3*column values */
static size_t  col_cap  = 0;          /* capacity in columns */
static size_t  col_used = 0;          /* columns already computed */

/* ensure col_cap is at least `needed` */
static void ensure_col_capacity(size_t needed)
{
    if (needed <= col_cap) return;
    size_t new_cap = col_cap ? col_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(col_vals, new_cap * 3 * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    col_vals = tmp;
    col_cap  = new_cap;
}

/* compute columns up to and including `n_col` */
static void compute_columns_upto(idx_t n_col)
{
    /* initialise column 0 if necessary */
    if (col_used == 0) {
        ensure_col_capacity(1);
        col_vals[0] = (seq_t)0;   /* T(0,0) */
        col_vals[1] = (seq_t)0;   /* T(0,1) */
        col_vals[2] = (seq_t)1;   /* T(0,2) */
        col_used   = 1;
    }

    /* compute new columns */
    while (col_used <= (size_t)n_col) {
        size_t p = (col_used - 1) * 3;  /* previous column */
        size_t c = col_used * 3;        /* current  column */

        /* T(k,0) = T(k-1,1) */
        col_vals[c + 0] = col_vals[p + 1];

        /* T(k,1) = T(k-1,0) + T(k-1,2) */
        col_vals[c + 1] = col_vals[p + 0] + col_vals[p + 2];

        /* T(k,2) = T(k-1,1) + T(k-1,0) */
        col_vals[c + 2] = col_vals[p + 1] + col_vals[p + 0];

        ++col_used;
    }
}

seq_t a(idx_t n)
{
    idx_t col = n / 3;
    idx_t row = n % 3;                     /* 0=bottom,1=middle,2=top */

    ensure_col_capacity((size_t)col + 1);
    compute_columns_upto(col);

    return col_vals[(size_t)col * 3 + (size_t)row];
}
