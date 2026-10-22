/*
 * A385895  Table read by rows: T(n, k) = T(n, k-1) + m * T(n-1, n-k) for k >
 *          1, T(n, 1) = T(n-1, n-1), and T(n, 0) = 0^n, for m = 2.
 * https://oeis.org/A385895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385895 – Triangle T(n,k) with m = 2
 * T(n,0)   = 0^n           (T(0,0) = 1, T(n,0) = 0 for n>0)
 * T(n,1)   = T(n-1,n-1)
 * T(n,k)   = T(n,k-1) + 2 * T(n-1,n-k)   for k > 1
 * The sequence a(n) is obtained by reading the triangle row‑by‑row
 * (row 0 first, then row 1, …).  a(0) = 1, a(1) = 0, a(2) = 1, ...
 * This implementation builds the rows on demand and caches them
 * (O(n²) storage for the rows) so that any a(n) can be retrieved in
 * O(1) after its row has been computed.  All arithmetic is performed
 * in seq_t – an unsigned integer type supplied by the build (a
 * fixed‑size or arbitrary‑precision type).  No floating‑point or
 * external libraries are used aside from the standard C headers.
 */

#include <stdlib.h>

/* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t row_of(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Grow high until the upper bound exceeds n. */
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        if (high > n) break;           /* safety guard for overflow */
        high <<= 1;
    }

    /* binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Determine row and column of n in the triangle. */
    idx_t r   = row_of(n);
    idx_t first = r * (r + 1) / 2;        /* first index of row r */
    idx_t col = n - first;                /* column inside the row   */

    /* Static cache of computed rows. */
    static seq_t **tri = NULL;            /* array of row pointers   */
    static idx_t tri_rows = 0;            /* highest row computed   */

    /* Allocate row array on first use. */
    if (tri == NULL) {
        tri = (seq_t**)malloc(sizeof(seq_t*));
        tri_rows = 0;
    }
    /* Grow the row array if we need to reach row r. */
    if (r >= tri_rows) {
        /* Ensure the array of pointers is large enough. */
        if (tri_rows == 0) {
            tri = (seq_t**)realloc(tri, (r + 1) * sizeof(seq_t*));
        } else {
            tri = (seq_t**)realloc(tri, (r + 1) * sizeof(seq_t*));
        }

        /* Create row 0 if it does not exist yet. */
        if (tri[0] == NULL) {
            tri[0] = (seq_t*)malloc(sizeof(seq_t));
            tri[0][0] = (seq_t)1;            /* 0^0 = 1 */
        }

        /* Compute new rows consecutively. */
        for (idx_t i = tri_rows + 1; i <= r; ++i) {
            /* Allocate space for row i (i+1 elements). */
            tri[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));

            /* Base cases. */
            tri[i][0] = (seq_t)0;             /* 0^i = 0 for i > 0 */
            tri[i][1] = tri[i-1][i-1];        /* T(n,1) = T(n-1,n-1) */

            /* Recurrence for k >= 2. */
            for (idx_t k = 2; k <= i; ++k) {
                tri[i][k] = tri[i][k-1] + (seq_t)2 * tri[i-1][i - k];
            }
        }
        tri_rows = r;  /* highest row now computed */
    }

    return tri[r][col];
}
