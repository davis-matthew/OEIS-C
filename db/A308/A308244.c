/*
 * A308244  Triangle T(n,k) read by rows, giving even-numbered coefficients of
 *          the matching polynomial of the n-ladder graph.
 * https://oeis.org/A308244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened sequence of triangle T(n,k) defined by
 *            T(0,0) = 1, T(1,0) = −1, T(1,1) = 1; T(n,k) = T(n-1,k-1) −
 *            2*T(n-1,k) − T(n-2,k-1) + T(n-3,k) (n ≥ 3) with the convention
 *            T(n,k)=0 unless 0 ≤ k ≤ n. a(n) is obtained by indexing the
 *            triangle row‑wise: let r be the unique integer with r(...
 * Range:     i8 n <= 22, i16 n <= 68, i32 n <= 236, i64 n <= 832,
 *            i128 n <= 3108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* dynamic storage for the triangle */
static seq_t **triangle = NULL;
static idx_t   rows_allocated = 0;   /* number of rows already computed */
static size_t rows_capacity   = 0;    /* capacity of the triangle pointer array */

/* grow the triangle pointer array to hold at least 'needed' rows */
static void grow_triangle_capacity(size_t needed)
{
    if (rows_capacity >= needed)
        return;
    size_t new_cap = rows_capacity ? rows_capacity * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    triangle = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
    rows_capacity = new_cap;
}

/* compute rows from rows_allocated up to and including row 'r' */
static void compute_row(idx_t r)
{
    /* ensure triangle pointer array has enough capacity */
    grow_triangle_capacity((size_t)r + 1);

    for (idx_t i = rows_allocated; i <= r; ++i) {
        /* allocate storage for row i (length i+1) */
        size_t len = (size_t)i + 1;
        seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!row)
            abort(); /* out of memory */

        triangle[i] = row;

        /* base rows */
        if (i == 0) {
            row[0] = 1;
            continue;
        }
        if (i == 1) {
            row[0] = -1;
            row[1] = 1;
            continue;
        }

        /* general recurrence */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t a = 0, b = 0, c = 0, d = 0;

            /* T(i-1,k-1) */
            if (i >= 1 && k > 0)
                a = triangle[i - 1][k - 1];

            /* T(i-1,k) */
            if (i >= 1 && k <= i - 1)
                b = triangle[i - 1][k];

            /* T(i-2,k-1) – only exists when k-1 ≤ i-2, i.e. k ≤ i-1 */
            if (i >= 2 && k > 0 && k <= i - 1)
                c = triangle[i - 2][k - 1];

            /* T(i-3,k) – only exists when k ≤ i-3 */
            if (i >= 3 && k <= i - 3)
                d = triangle[i - 3][k];

            row[k] = a - 2 * b - c + d;
        }
    }
    rows_allocated = r + 1;
}

seq_t a(idx_t n)
{
    if (n < 0)
        return 0; /* not expected, but safe */

    /* determine row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    long double val = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);

    /* adjust r in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    idx_t col = n - r * (r + 1) / 2;

    /* ensure the triangle is built up to row r */
    if (rows_allocated <= r)
        compute_row(r);

    return triangle[r][col];
}
