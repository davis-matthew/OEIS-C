/*
 * A391467  Triangle read by rows: T(n,0) = T(n,n) = 0 and T(n,k) = T(n, k-1)
 *          + T(n-1, k-1) + T(n-2, k-1) + k for 0 < k < n.
 * https://oeis.org/A391467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where n = r*(r+1)/2 + k (0 ≤ k ≤ r) * T(0,0)=0; *
 *            T(n,0)=T(n,n)=0; * T(n,k)=T(n,k-1)+T(n-1,k-1)+T(n-2,k-1)+k
 *            (0<k<n) * * Initial terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=0, a(6)=0, * a(7)=1, a(8)=4,
 * Range:     u8 n <= 41, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Helper: triangular number: start index of row r in a linear array of rows */
static inline idx_t tri_start(idx_t r) {
    return r * (r + 1) / 2;
}

/* Persistent storage for the triangle: linear array of T(r,k) values */
static seq_t *triangle = NULL;      /* linear storage */
static idx_t  max_row_computed = 0; /* highest row that has been computed */
static size_t triangle_capacity = 0;

/* Ensure that rows 0..up_to have been computed and stored in 'triangle'. */
static void ensure_rows(idx_t up_to) {
    if (up_to <= max_row_computed) return;

    /* Allocate enough space for all rows up to 'up_to' (inclusive): */
    size_t needed = (size_t)( (up_to + 1) * (up_to + 2) / 2 );
    if (triangle_capacity < needed) {
        if (triangle == NULL)
            triangle = (seq_t *)malloc(needed * sizeof(seq_t));
        else
            triangle = (seq_t *)realloc(triangle, needed * sizeof(seq_t));
        if (!triangle) abort();          /* allocation failed */
        triangle_capacity = needed;
    }

    /* Compute the new rows, starting from max_row_computed + 1. */
    for (idx_t r = max_row_computed + 1; r <= up_to; ++r) {
        idx_t start = tri_start(r);
        for (idx_t k = 0; k <= r; ++k) {
            if (k == 0 || k == r) {
                triangle[start + k] = 0;
            } else {
                seq_t v_left  = triangle[start + k - 1];                         /* T(r,k-1)   */
                seq_t v_up1   = triangle[tri_start(r - 1) + k - 1];              /* T(r-1,k-1) */
                seq_t v_up2   = triangle[tri_start(r - 2) + k - 1];              /* T(r-2,k-1) */
                triangle[start + k] = v_left + v_up1 + v_up2 + (seq_t)k;
            }
        }
    }
    max_row_computed = up_to;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Handle the trivial case immediately. */
    if (n == 0) return 0;

    /* Find the row r such that tri_start(r) ≤ n < tri_start(r+1). */
    double d = sqrt(1.0 + 8.0 * (double)n);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors. */
    while (tri_start(r) > n) r--;
    while (tri_start(r + 1) <= n) r++;

    idx_t k = n - tri_start(r);

    ensure_rows(r);

    return triangle[tri_start(r) + k];
}
