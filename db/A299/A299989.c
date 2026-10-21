/*
 * A299989  Triangle read by rows: T(n,0) = 0 for n >= 0; T(n,2*k+1) =
 *          A152842(2*n,2*(n-k)) and T(n,2*k) = A152842(2*n,2*(n-k)+1) for n
 *          >= k > 0.
 * https://oeis.org/A299989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, nonn
 * Formula:   a(n) = T(r,k) where (r,k) are obtained from the * row‑major
 *            encoding of the triangle of coefficients of * x*(x^2 + 4*x +
 *            3)^r. * * For a given index n: * r = floor((sqrt(4*n+1)-1)/2)
 *            (then corrected for rounding) * k = n - r*(r+1) * * Triangle
 *            definition: * T(0,0)=0, T(0,1)=1 * T(n,k) = T(n-1,k-2...
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 162, u64 n <= 564,
 *            u128 n <= 2010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t **triangle = NULL;  /* triangle[r][k] */
static idx_t triangle_rows = 0;  /* number of rows currently cached */

/* Build rows 0..r of the triangle and cache them. */
static void ensure_triangle(idx_t r)
{
    if (r < triangle_rows)
        return;

    /* Allocate or grow the array of row pointers. */
    triangle = (seq_t **)realloc(triangle, (size_t)(r + 1) * sizeof(seq_t *));
    for (idx_t i = triangle_rows; i <= r; ++i) {
        size_t len   = 2 * i + 2;            /* number of coefficients in row i */
        seq_t *row   = (seq_t *)malloc(len * sizeof(seq_t));
        for (size_t k = 0; k < len; ++k)
            row[k] = 0;

        if (i == 0) {                       /* base row */
            row[0] = 0;
            row[1] = 1;
        } else {
            seq_t *prev     = triangle[i - 1];
            size_t prev_len = 2 * (i - 1) + 2;
            for (size_t k = 0; k < len; ++k) {
                seq_t val = 0;
                if (k >= 2 && k - 2 < prev_len)   /* T(n-1,k-2) */
                    val += prev[k - 2];
                if (k >= 1 && k - 1 < prev_len)   /* 4*T(n-1,k-1) */
                    val += 4 * prev[k - 1];
                if (k < prev_len)                 /* 3*T(n-1,k)  */
                    val += 3 * prev[k];
                row[k] = val;
            }
        }
        triangle[i] = row;
    }
    triangle_rows = r + 1;
}

/* Sequence function required by the build system. */
seq_t a(idx_t n)
{
    /* Handle the trivial first row directly. */
    if (n < 2) {
        ensure_triangle(0);
        return triangle[0][n];
    }

    /* Estimate r from the quadratic formula, then correct. */
    double xd = (double)n;                              /* may lose precision for huge n */
    idx_t r   = (idx_t)((sqrt(4.0 * xd + 1.0) - 1.0) * 0.5);

    /* Adjust in case the floating‑point estimate is off. */
    while (((unsigned __int128)(r + 1) * (r + 2)) <= n) ++r;
    while (((unsigned __int128)r * (r + 1)) > n) --r;

    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1));

    ensure_triangle(r);
    return triangle[r][k];
}
