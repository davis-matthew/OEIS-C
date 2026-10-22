/*
 * A342708  Triangle read by rows with n >= 1, 0 <= k <= n-1: in general
 *          T(n,k) = T(n-1,k-1) - T(n-k,k), except T(1,0)=1, T(n,0)=0 for
 *          n>=2; T(n,k) = 0 for k>=n.
 * https://oeis.org/A342708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 2289, i16 n <= 9753, i32 n <= 9753, i64 n <= 9753,
 *            i128 n <= 9753, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342708
    Triangle T(n,k) = T(n-1,k-1) – T(n-k,k);
    T(1,0)=1,  T(n,0)=0 for n≥2,   T(n,k)=0 for k≥n.
    The sequence a(n) is the triangle read by rows (row 1 has 1 entry,
    row 2 has 2 entries, …).  Thus a(n)=T(r,k) where
        r*(r-1)/2 < n ≤ r*(r+1)/2   and   k = n – r*(r-1)/2 – 1.
*/

#include <stdlib.h>

/*=== static storage for the running triangle ===*/
static seq_t *tri = NULL;      /* array of all computed entries        */
static size_t tri_cap = 0;     /* current allocation size in seq_t's  */
static size_t tri_len = 0;     /* number of entries actually stored    */
static idx_t  tri_max_row = 0; /* largest n for which rows have been    */
/*   computed (tri_max_row ≥ 0)                  */

/* ensure the array can hold at least needed slots */
static void ensure_capacity(size_t needed)
{
    if (needed <= tri_cap) return;

    size_t new_cap = tri_cap ? tri_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory error */
    tri       = tmp;
    tri_cap   = new_cap;
}

/*=== the required function ===*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n<1 */
    /* ensure that rows 1..tri_max_row cover index n */
    while (tri_max_row * (tri_max_row + 1) / 2 < n)
    {
        /* next row to compute: r = tri_max_row + 1 */
        idx_t r      = ++tri_max_row;
        size_t start = (size_t)r * (size_t)(r - 1) / 2;   /* first index of row r (0‑based) */
        size_t needed = start + (size_t)r;
        ensure_capacity(needed);

        /* compute all entries of row r */
        for (idx_t k = 0; k < r; ++k)
        {
            size_t pos = start + (size_t)k;
            seq_t val;

            if (r == 1 && k == 0)            /* base case */
                val = 1;
            else if (k == 0)                 /* T(n,0)=0 for n≥2 */
                val = 0;
            else
            {
                /* T1 = T(r-1,k-1)  (exists because k>0) */
                size_t start_prev = (size_t)(r - 2) * (size_t)(r - 1) / 2;
                seq_t T1 = tri[start_prev + (size_t)(k - 1)];

                /* T2 = T(r-k,k)  only if k < r-k */
                seq_t T2 = 0;
                idx_t row_nk = r - k;
                if ((idx_t)k < row_nk)   /* otherwise T(n,k)=0 for k≥n */
                {
                    size_t start_nk = (size_t)(row_nk - 1) * (size_t)row_nk / 2;
                    T2 = tri[start_nk + (size_t)k];
                }

                val = T1 - T2;
            }
            tri[pos] = val;
        }
        tri_len = needed;
    }
    return tri[n - 1];   /* flattening uses 0‑based index */
}
