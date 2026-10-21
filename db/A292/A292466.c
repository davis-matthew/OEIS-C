/*
 * A292466  Triangle read by rows: T(n,k) = 4*T(n-1,k-1) + T(n,k-1) with
 *          T(2*m,0) = 0 and T(2*m+1,0) = 5^m.
 * https://oeis.org/A292466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 103, u64 n <= 375,
 *            u128 n <= 1323, big any n
 * Helpers:   pow5_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A292466 – Triangle read by rows
 *
 *  Recurrence:
 *      T(n,k) = 4*T(n-1,k-1) + T(n,k-1)   (k>0)
 *      T(2*m,0)   = 0
 *      T(2*m+1,0) = 5^m
 *
 *  Sequence a(n) is obtained by reading the triangle by rows.
 *  The function memoises computed rows and is thread‑safe in the sense
 *  that it can be called in any order.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow5_u.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Helper: find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
static idx_t row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* grow hi until we exceed n */
    while (( (__int128)hi * (hi + 1) ) / 2 <= n)
        hi <<= 1;
    /* binary search */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) ) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
seq_t a(idx_t n)
{
    /* static storage for the triangle */
    static seq_t *tri   = NULL;   /* linear array of all rows */
    static idx_t tri_sz = 0;      /* allocated size (in seq_t units) */
    static idx_t cur_row = 0;     /* highest row computed so far */

    /* locate the target row and column */
    idx_t r = row_of(n);          /* row number   */
    idx_t offset = (r * (r + 1)) / 2;    /* offset of first element of row r */
    idx_t c = n - offset;         /* column index */

    /* initialise storage if first call */
    if (tri == NULL) {
        tri_sz = 1;
        tri = (seq_t*)malloc(tri_sz * sizeof(seq_t));
        if (tri == NULL) abort();
        tri[0] = (seq_t)0;       /* T(0,0) = 0 */
        cur_row = 0;
    }

    /* extend triangle up to row r */
    if (r > cur_row) {
        for (idx_t i = cur_row + 1; i <= r; ++i) {
            idx_t start     = (i * (i + 1)) / 2;            /* first index of row i */
            idx_t needed_sz = start + i + 1;               /* indices up to last element */

            if (needed_sz > tri_sz) {
                tri_sz = needed_sz;
                seq_t *tmp = (seq_t*)realloc(tri, tri_sz * sizeof(seq_t));
                if (tmp == NULL) abort();
                tri = tmp;
            }

            /* first element of the row */
            if ((i & (idx_t)1) == 0)                     /* even i -> 0 */
                tri[start] = (seq_t)0;
            else                                         /* odd i = 2*m+1 */
                tri[start] = pow5_u((i - 1) / 2);

            /* remaining elements of the row */
            idx_t prev_start = ((i - 1) * i) / 2;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t t_prev_prev = tri[prev_start + (k - 1)];
                seq_t t_prev_curr = tri[start + (k - 1)];
                tri[start + k] = t_prev_prev * (seq_t)4 + t_prev_curr;
            }
        }
        cur_row = r;
    }

    return tri[offset + c];
}
