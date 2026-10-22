/*
 * A338131  Triangle read by rows, T(n, k) = k^(n - k) + Sum_{i = 1..n-k} k^(n
 *          - k - i)*2^(i - 1), for 0 <= k <= n.
 * https://oeis.org/A338131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where n is the flattened index of the triangle *
 *            T(n,k) = k^(n-k) + Σ_{i=1..n-k} k^(n-k-i)·2^(i-1) (k ≤ n). *
 *            Recurrence: T(n+1,k) = k·T(n,k) + 2^(n-k) (0 ≤ k ≤ n). * Base:
 *            T(0,0) = 1, T(n,n) = 1
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Compute the row `r` such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t compute_row(idx_t n)
{
    idx_t r = 0;
    /* Linear scan is cheap: for n ≤ 10⁴ r ≤ 141; for larger n it is still fine. */
    while (1) {
        /* Use 128‑bit arithmetic to avoid overflow for very large n. */
        unsigned __int128 lhs = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (lhs > n)
            break;
        ++r;
    }
    return r;
}

/* Cache of the triangle.  tri[r][k] = T(r,k). */
static seq_t **tri = NULL;            /* array of row pointers */
static idx_t rows_allocated = 0;       /* number of rows already computed */

/* Ensure rows 0..max_r (inclusive) exist and are filled. */
static void ensure_rows(idx_t max_r)
{
    if (max_r < rows_allocated)
        return;

    /* Grow the array of row pointers. */
    seq_t **new_tri =
        (seq_t **)realloc(tri, (size_t)(max_r + 1) * sizeof(seq_t *));
    if (!new_tri && max_r + 1 > 0) {
        abort(); /* allocation failure */
    }
    tri = new_tri;

    /* Compute each new row. */
    for (idx_t r = rows_allocated; r <= max_r; ++r) {
        /* Allocate the k component of this row. */
        tri[r] = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
        if (!tri[r]) {
            abort();
        }

        if (r == 0) {
            tri[0][0] = (seq_t)1;
        } else {
            for (idx_t k = 0; k < r; ++k) {
                seq_t prev = tri[r - 1][k];
                seq_t add = pow2((idx_t)(r - 1 - k)); /* 2^(r-1-k) */
                tri[r][k] = (seq_t)k * prev + add;
            }
            tri[r][r] = (seq_t)1; /* T(r,r) = 1 */
        }
    }
    rows_allocated = max_r + 1;
}

seq_t a(idx_t n)
{
    /* Find row r and column k from the flattened index n. */
    idx_t r = compute_row(n);
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* Compute missing rows. */
    if (r >= rows_allocated) {
        ensure_rows(r);
    }

    return tri[r][k];
}
