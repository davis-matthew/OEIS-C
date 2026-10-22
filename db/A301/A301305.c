/*
 * A301305  G.f. L(x,y) satisfies: L(x,y) = x * (1 + y*x*L'(x,y)) / (1 +
 *          x*L'(x,y)) where L'(x,y) = d/dx L(x,y), as a triangle read by
 *          rows.
 * https://oeis.org/A301305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(1)=1, and for n>=2 the coefficients
 *            a_n(y)=∑_{k=0}^{n-1}T(n,k)y^k of the generating function L(x,y)
 *            satisfy L + x L L' - y x² L' - x = 0. The algorithm constructs
 *            the triangle T(n,k) row‑by‑row using this recurrence, then
 *            answers a(n) by indexing the appropriate entry
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 58, i64 n <= 157,
 *            i128 n <= 417, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *rows[2001];       /* stored rows of the triangle; rows[0] unused */
static size_t rows_len[2001];   /* length of each stored row */
static idx_t rows_computed = 0; /* highest row computed so far */

/* ensure that rows 1..target are computed */
static void ensure_rows(idx_t target)
{
    if (target <= rows_computed)
        return;

    for (idx_t r = rows_computed + 1; r <= target; ++r) {
        size_t len = (size_t)r;
        rows[r] = (seq_t *)calloc(len, sizeof(seq_t));
        rows_len[r] = len;

        /* temporary array for the convolution term */
        seq_t *S = (seq_t *)calloc(len, sizeof(seq_t));

        /* Sum over j=1..r-1 of j * a_j * a_{r-j}, accumulated into S */
        for (size_t j = 1; j < r; ++j) {
            seq_t *aj    = rows[j];
            seq_t *a_rj  = rows[r - j];
            seq_t coef   = (seq_t)j;

            for (size_t i = 0; i < j; ++i) {
                seq_t temp = aj[i] * coef;
                for (size_t k = 0; k < r - j; ++k)
                    S[i + k] += temp * a_rj[k];
            }
        }

        /* a_r = -S (start with the negative of the convolution sum) */
        seq_t *ar = rows[r];
        for (size_t i = 0; i < len; ++i)
            ar[i] = -S[i];

        /* shift term: + (r-1) * shift(a_{r-1}) */
        if (r > 1) {
            seq_t *prev = rows[r - 1];
            seq_t shift_coef = (seq_t)(r - 1);
            for (size_t k = 0; k < r - 1; ++k)
                ar[k + 1] += shift_coef * prev[k];
        }

        /* base case correction */
        if (r == 1)
            ar[0] += (seq_t)1;

        free(S);
    }

    rows_computed = target;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r such that r*(r-1)/2 < n <= r*(r+1)/2   */
    idx_t r = (idx_t)((sqrt((long double)8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n)
        --r;

    /* } ensure the row exists
       the required coefficient has index k = n - r*(r-1)/2 - 1   */
    ensure_rows(r);

    idx_t k = n - (idx_t)((unsigned __int128)r * (r - 1) / 2) - 1;

    return rows[r][(size_t)k];
}
