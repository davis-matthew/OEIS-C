/*
 * A334465  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into k consecutive parts that differ by 5, and the
 *          first element of column k is in the row that is the k-th
 *          heptagonal number (A000566).
 * https://oeis.org/A334465
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

/*  OEIS A334465 – irregular triangle read by rows
 *
 *  The triangle T(r,k) is defined for r,k ≥ 1 by
 *     T(r,k) = 1  ⇔  r = k*a + 5*k*(k‑1)/2   with a ≥ 1 integer
 *             0  otherwise
 *
 *  The sequence a(n) is obtained by writing the triangle row by row
 *  (row 1, then row 2, then row 3, …).  In row r the number of
 *  columns is the largest k such that the k‑th heptagonal number
 *  H_k = k*(5k‑3)/2 does not exceed r.  The first element of column
 *  k appears in row H_k.
 *
 *  The function below returns the n‑th term a(n) (1‑based index) for
 *  any n ≥ 1.  It uses a memoised cumulative table cum[r] storing the
 *  total number of triangle entries up to row r.  All arithmetic is
 *  performed in the unsigned integer type seq_t; the index type idx_t
 *  is an unsigned 64‑bit integer.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*  Global state used to memoise the cumulative table.  */
static idx_t *cum        = NULL;      /* cum[0] = 0, cum[r] = total entries up to row r */
static size_t  cum_size  = 0;         /* capacity of cum */
static idx_t   max_r     = 0;         /* largest row currently computed */
static idx_t   cur_k     = 0;         /* number of columns in the current (max_r) row */
static idx_t   hept_next = 1;         /* next row where the column count increases */

/*  Ensure that cum[max_r] ≥ n and that cum has enough capacity.  */
static void
ensure_cum(idx_t n)
{
    /* initialise on first call */
    if (cum == NULL) {
        cum_size = 128;            /* initial capacity */
        cum = (idx_t *)malloc((cum_size + 1) * sizeof(idx_t));
        if (cum == NULL) abort();
        cum[0] = 0;
        max_r   = 0;
        cur_k   = 0;
        hept_next = 1;
    }

    /* extend the table until we have enough entries */
    while (cum[max_r] < n) {
        /* grow the array if required */
        if ((size_t)(max_r + 1) > cum_size) {
            cum_size <<= 1;
            cum = (idx_t *)realloc(cum, (cum_size + 1) * sizeof(idx_t));
            if (cum == NULL) abort();
        }

        ++max_r;   /* new row number */

        /* Increase column count when crossing a heptagonal threshold */
        if (max_r >= hept_next) {
            ++cur_k;
            hept_next = (cur_k + 1) * (5 * (cur_k + 1) - 3) / 2;
        }

        cum[max_r] = cum[max_r - 1] + cur_k;  /* cumulative entries up to this row */
    }
}

/*  Return the n‑th term of A334465.  */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    /* build cumulative table if necessary */
    ensure_cum(n);

    /* binary search for the row r such that cum[r‑1] < n ≤ cum[r] */
    idx_t lo = 1, hi = max_r;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;

    /* column index within the row */
    idx_t c = n - cum[r - 1];

    /* trivial partition always exists for column 1 */
    if (c == 1) return (seq_t)1;

    /* check existence of a partition into c consecutive parts diff 5 */
    idx_t temp = 5 * c * (c - 1) / 2;              /* 5*c*(c‑1)/2  */
    /* first row with a ≥ 1 is H_c = c*(5c‑3)/2 = temp + c */
    if (r < temp + c) return (seq_t)0;

    idx_t numer = r - temp;
    if (numer % c != 0) return (seq_t)0;          /* a not integer */

    return (seq_t)1;    /* a ≥ 1 guaranteed by the previous check */
}
