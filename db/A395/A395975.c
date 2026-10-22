/*
 * A395975  Triangle read by rows: matrix inverse of triangle A126216 with a
 *          left unit column adjoined.
 * https://oeis.org/A395975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 28, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle inverse recurrence:
     T(n,n) = 1
     T( n, k ) = -∑_{j=k+1}^{n} T(n,j) · C(j-1,k-1) · C(2j-k+1,j-k+1) / (j+1)
     for k < n.
   The sequence a(n) is obtained by listing the rows of T
   consecutively:  a(0)=T(0,0),  a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/*  Storage for rows of the inverse triangle.
    Rows are allocated on demand and cached.  */
static seq_t **T_rows = NULL;          /* array of pointers to rows */
static idx_t  T_rows_cap = 0;           /* current capacity of T_rows */

/*  Ensure that T_rows can store rows 0 … need (inclusive).  */
static void T_ensure(idx_t need)
{
    if (need < T_rows_cap) return;

    size_t new_cap = T_rows_cap ? T_rows_cap * 2 : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    seq_t **tmp = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
    if (!tmp) abort();
    for (size_t i = T_rows_cap; i < new_cap; ++i) tmp[i] = NULL;

    T_rows     = tmp;
    T_rows_cap = (idx_t)new_cap;
}

/*  Compute the n‑th row of T and store it in T_rows[n].  */
static void T_compute(idx_t n)
{
    /* Allocate space for n+1 entries (indices 0 … n). */
    seq_t *row = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!row) abort();

    row[n] = (seq_t)1;   /* diagonal entry */

    /* Backward construction: for k = n-1 … 0 */
    for (idx_t k = n; k-- > 0; )
    {
        seq_t sum = (seq_t)0;

        for (idx_t j = k + 1; j <= n; ++j)
        {
            seq_t bin1;
            if (k == 0) {
                bin1 = (seq_t)0;      /* C(j-1,-1) is treated as 0 */
            } else {
                bin1 = binomial(j - 1, k - 1);
            }
            if (bin1 == (seq_t)0) continue;

            seq_t bin2 = binomial(2 * j - k + 1, j - k + 1);

            seq_t term = row[j];
            term *= bin1;
            term *= bin2;
            term /= (seq_t)(j + 1);

            sum += term;
        }
        row[k] = -sum;   /* negative of the sum */
    }

    T_rows[n] = row;
}

/*  Flattened row‑by‑row sequence a(n).  */
seq_t a(idx_t n)
{
    /* Find row r such that cumulative size up to r‑1 ≤ n < cumulative size up to r. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t c = n - cum;  /* column within the row */

    T_ensure(r);
    if (!T_rows[r]) T_compute(r);

    return T_rows[r][c];
}
