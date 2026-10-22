/*
 * A394813  Triangle read by rows: T(n, k) is the k-th even-indexed column of
 *          the array A394582.
 * https://oeis.org/A394813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the linear reading of the triangle T(n,k) defined by
 *            T(n,1) = n, T(n,n) = 1, and for 1<k<n T(n,k) = (n-k+1)^2 *
 *            T(n-1,k-1) + (n-k+1) * Σ_{j=1..n-k} ( j * T(j+k-2, k-1) )
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 61, u64 n <= 162,
 *            u128 n <= 395, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are provided by the build environment:
   typedef uint64_t idx_t;          // unsigned 64‑bit index
   typedef unsigned __int128 seq_t; // or any other unsigned type
   The function a(idx_t) is declared externally. */

/* Static storage for the triangle (1‑based indexing). */
static seq_t **triangle = NULL;  /* rows 0..current_max_row are allocated */
static idx_t  current_max_row = 0;   /* largest row computed so far */

/* Find the smallest r with r*(r+1)/2 >= n, using 128‑bit arithmetic to avoid overflow. */
static inline idx_t find_row(idx_t n)
{
    idx_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n )
        ++r;
    return r;
}

/* Ensure that rows up to 'up_to' have been computed. */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= current_max_row)
        return;

    /* Resize the pointer array if necessary. */
    if (triangle == NULL) {
        triangle = (seq_t **)calloc(up_to + 1, sizeof(seq_t *));
    } else {
        seq_t **tmp = (seq_t **)realloc(triangle, (up_to + 1) * sizeof(seq_t *));
        if (tmp == NULL)
            exit(EXIT_FAILURE);  /* allocation failed */
        triangle = tmp;
    }

    /* Compute each new row. */
    for (idx_t n = current_max_row + 1; n <= up_to; ++n) {
        /* Allocate space for this row (indices 0..n). */
        triangle[n] = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (triangle[n] == NULL)
            exit(EXIT_FAILURE);
        /* Base cases */
        triangle[n][1] = (seq_t)n;
        triangle[n][n] = (seq_t)1;
        /* Fill interior entries when n > 2. */
        for (idx_t k = 2; k < n; ++k) {
            seq_t tmp     = (seq_t)(n - k + 1);
            seq_t tmpSq   = tmp * tmp;
            seq_t sumTerm = 0;
            for (idx_t j = 1; j <= n - k; ++j) {
                idx_t rowIdx = j + k - 2;           /* i in the recurrence */
                seq_t term   = (seq_t)j * triangle[rowIdx][k - 1];
                sumTerm     += term;
            }
            seq_t part1 = tmpSq * triangle[n - 1][k - 1];
            seq_t part2 = tmp * sumTerm;
            triangle[n][k] = part1 + part2;
        }
    }
    current_max_row = up_to;
}

/* External API: returns the n‑th term a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined; return 0 for safety */
    idx_t row = find_row(n);
    idx_t col = n - (row - 1) * row / 2;
    ensure_rows(row);
    return triangle[row][col];
}
