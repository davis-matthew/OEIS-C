/*
 * A333142  Triangle read by rows: T(n, k) = qStirling1(n, k, q) for q = 2,
 *          with 0 <= k <= n.
 * https://oeis.org/A333142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Sequence a(n) is obtained by reading the triangle * T(n,k) =
 *            qStirling1(n,k,2) by rows with offset 0. * * The triangle
 *            satisfies: * T(0,0) = 1 * T(n,0) = T(n,n) = 1 for n > 0 * T(n,k)
 *            = T(n-1,k-1) + (2^(n-1)-1)*T(n-1,k) * for 1 <= k < n * * a(n) =
 *            T(r,c) where r and c are determined by the positi...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

/* Maximum row index that has been computed */
static idx_t max_row = 0;

/* Triangle cache: tri[n] points to an array of length n+1 */
static seq_t **tri = NULL;
static size_t tri_cap = 0;

/* Compute all rows of the triangle up to 'target' (inclusive). */
static void compute_up_to_row(idx_t target)
{
    if (target <= max_row)
        return;

    /* Ensure storage for all rows up to target */
    if (tri_cap < (size_t)(target + 1)) {
        tri_cap = (size_t)(target + 1);
        tri = (seq_t **)realloc(tri, tri_cap * sizeof(seq_t *));
    }

    for (idx_t i = max_row + 1; i <= target; ++i) {
        size_t len = (size_t)(i + 1);
        tri[i] = (seq_t *)malloc(len * sizeof(seq_t));
        tri[i][0] = 1;          /* T(i,0) = 1 */
        tri[i][i] = 1;          /* T(i,i) = 1 */

        if (i == 0)
            continue;

        /* factor = 2^(i-1) - 1 */
        seq_t factor = pow2((idx_t)(i - 1));
        factor -= 1;

        for (idx_t k = 1; k < i; ++k) {
            /* T(i,k) = T(i-1,k-1) + factor * T(i-1,k) */
            tri[i][k] = tri[i - 1][k - 1] + factor * tri[i - 1][k];
        }
    }

    max_row = target;
}

/* Determine the row index for the given sequence index n.
 * Returns r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
 * Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t row_for_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Exponential search to find an upper bound */
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/*
 * External function required by the build system.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* special case; handled later as well */

    /* Find the row and column of the nth element */
    idx_t row = row_for_index(n);
    idx_t before = (idx_t)((__int128)row * (row + 1) / 2);
    idx_t col = n - before;

    /* Ensure the triangle is computed up to this row */
    compute_up_to_row(row);

    return tri[row][col];
}
