/*
 * A338817  Matrix inverse of triangle A176270, read by rows.
 * https://oeis.org/A338817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   T(n,k) = 1
 * Range:     i8 n <= 16, i16 n <= 30, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          T(n,n-1) = n-2                                     */
/*          T(n,n-2) = n^2-3n+1                               */
/*          T(n,k) = (k^2+k-1) * n! / (k+2)!  for 0≤k≤n-2   */
/* The sequence a(n) is the triangle T read by rows.          */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fact   = NULL; /* factorial array, fact[i] = i! */
static idx_t  fact_n = 0;    /* number of factorials available (num-1) */

static seq_t **T          = NULL; /* rows of triangle, T[n][k] */
static idx_t   t_rows     = 0;    /* number of rows computed (num-1) */

/* Ensure factorials up to idx (inclusive) */
static void ensure_factorial(idx_t idx)
{
    if (idx < fact_n) return;
    size_t new_cap = (size_t)(idx + 1);
    fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!fact) abort();
    for (idx_t i = fact_n; i <= idx; ++i) {
        if (i == 0) fact[i] = 1;
        else        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_n = idx + 1;
}

/* Ensure triangle rows up to idx (inclusive) */
static void ensure_triangle(idx_t idx)
{
    if (idx < t_rows) return;
    size_t new_rows = (size_t)(idx + 1);
    T = (seq_t **)realloc(T, new_rows * sizeof(seq_t *));
    if (!T) abort();
    for (idx_t n = t_rows; n <= idx; ++n) {
        /* allocate row of length n+1 */
        seq_t *row = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (!row) abort();
        T[n] = row;

        /* compute entries */
        for (idx_t k = 0; k <= n; ++k) {
            if (k == n) {                /* main diagonal */
                row[k] = 1;
            } else if (k == n - 1) {     /* super‑diagonal */
                row[k] = (seq_t)((int64_t)n - 2);
            } else if (k == n - 2) {     /* second super‑diagonal */
                row[k] = (seq_t)((int64_t)n * (int64_t)n
                                 - 3 * (int64_t)n + 1);
            } else {                     /* general case */
                /* ensure factorials for used indices */
                ensure_factorial(n);
                /* (k^2 + k - 1) as signed, then cast */
                int64_t t = (int64_t)k * (int64_t)k
                            + (int64_t)k - 1;
                seq_t val = (seq_t)t;            /* signed value */
                val *= fact[n];                  /* n!  */
                val /= fact[k + 2];              /* divide by (k+2)! */
                row[k] = val;
            }
        }
    }
    t_rows = idx + 1;
}

/* Return a(n) by reading the triangle row by row */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* make sure we have all required rows */
    ensure_triangle(r);

    /* locate within row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;   /* column */
    return T[r][c];
}
