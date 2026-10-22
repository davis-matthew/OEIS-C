/*
 * A300729  Number of arrangements on a line of n finite closed intervals
 *          (possibly of zero length) with k distinct endpoints (n >= 1, 1 <=
 *          k <= 2*n).
 * https://oeis.org/A300729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A300729 – Number of arrangements on a line of n
 * finite closed intervals (possibly of zero length) with k distinct endpoints.
 *
 * This implementation computes the sequence a(n) by building the
 * triangular array T(r,k) using the recurrence
 *
 *   T(r,k) = k*(k+1)/2 * T(r-1,k)
 *          + k*k          * T(r-1,k-1)
 *          + k*(k-1)/2    * T(r-1,k-2)
 *
 * with base row r = 1, k = 1..2 :  T(1,1) = T(1,2) = 1.
 *
 * The global sequence a(n) is obtained by reading the rows in order:
 *   (r=1, k=1), (r=1, k=2),
 *   (r=2, k=1), (r=2, k=2), (r=2, k=3), (r=2, k=4),
 *   (r=3, k=1), …, (r=3, k=6), …
 *
 * Only dynamic memory allocation is used, so the code is valid in both
 * C11 and C++17.
 */

#include <stdlib.h>

/*  Compute a(n) where n >= 1  */
seq_t a(idx_t n)
{
    /* Local static state to cache computed values between calls. */
    static seq_t *cache = NULL;          /* a(1), a(2), …  */
    static idx_t cache_len = 0;          /* number of elements stored (1‑based) */
    static idx_t cache_cap = 0;

    static seq_t *prev = NULL;           /* previous row T(r-1, *)  */
    static idx_t prev_len = 0;           /* size of prev (index 0..2*(r-1)) */
    static idx_t current_row = 0;        /* r such that prev holds T(r, *) */
    static idx_t total_indices = 0;      /* highest sequence index computed */

    /* Helper to allocate or grow the cache. */
    if (n > total_indices) {
        /* Ensure the cache can hold at least n entries (1‑based). */
        if (cache_cap < n + 1) {
            size_t new_cap = cache_cap > 0 ? cache_cap : 16;
            while (new_cap < (size_t)(n + 1))
                new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Initialise the structure on the first call. */
        if (cache_len == 0) {
            /* Row r = 1 : T(1,1) = 1, T(1,2) = 1 */
            current_row = 1;
            prev_len = 3;                /* indices 0 .. 2*1 */
            prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
            prev[0] = 0;                 /* unused but convenient  */
            prev[1] = 1;
            prev[2] = 1;

            cache_len = 2;               /* we will fill cache[1] and cache[2] */
            cache_cap = (cache_cap > 0) ? cache_cap : 16;
            cache[1] = 1;
            cache[2] = 1;
            total_indices = 2;
        }

        /* Compute successive rows until we have index n. */
        while (total_indices < n) {
            idx_t r = current_row + 1;      /* next row number */
            idx_t row_size = 2 * r + 1;     /* allocate 0 .. 2r */

            seq_t *curr = (seq_t *)malloc(row_size * sizeof(seq_t));
            curr[0] = 0;                    /* unused placeholder */

            for (idx_t k = 1; k <= 2 * r; ++k) {
                seq_t term1 = 0, term2 = 0, term3 = 0;

                /* T(r-1, k) */
                if (k < prev_len) {
                    term1 = ((seq_t)k * (k + 1) / 2) * prev[k];
                }

                /* T(r-1, k-1) */
                if (k >= 1 && (k - 1) < prev_len) {
                    term2 = ((seq_t)k * k) * prev[k - 1];
                }

                /* T(r-1, k-2) */
                if (k >= 2 && (k - 2) < prev_len) {
                    term3 = ((seq_t)k * (k - 1) / 2) * prev[k - 2];
                }

                curr[k] = term1 + term2 + term3;
            }

            /* Append the new row to the cache. */
            if (total_indices + 2 * r > cache_cap) {
                size_t new_cap = cache_cap > 0 ? cache_cap : 16;
                while (new_cap < (size_t)(total_indices + 2 * r))
                    new_cap <<= 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }

            for (idx_t k = 1; k <= 2 * r; ++k) {
                cache[++total_indices] = curr[k];
            }

            /* Prepare for next iteration. */
            free(prev);
            prev = curr;
            prev_len = row_size;
            current_row = r;
        }
    }

    return cache[n];
}
