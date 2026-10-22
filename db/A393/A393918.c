/*
 * A393918  Triangle read by rows: T(n, k) is the number of compositions of n
 *          into parts at least k.
 * https://oeis.org/A393918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of compositions of n into parts at least k, read
 *            by rows of T(n,k) where T(n,k) satisfies T(n,k) = T(n-1,k) +
 *            T(n-k,k) (n ≥ k), with the single base value T(k,k) = 1. This
 *            yields the triangle read row‑wise: 1; 2,1; 4,1,1; 8,2,1,1,1; …
 *            The sequence a(n) is obtained by mapping the ind...
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t compute_row(idx_t n) {
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    return r;
}

seq_t a(idx_t n) {
    static idx_t max_row = 0;          /* highest row fully computed  */
    static seq_t *cache = NULL;        /* flattened triangular array  */
    static size_t cache_size = 0;      /* current capacity (half‑square) */

    /* find the row containing a(n) */
    idx_t r = compute_row(n);
    size_t row_start = (size_t)(r - 1) * r / 2;  /* first index of row r */

    /* extend the cache to include rows up to r, if necessary */
    if (max_row < r) {
        /* new capacity */
        size_t new_cap = (size_t)r * (r + 1) / 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – the caller should handle this */
            return 0;
        }
        cache = new_cache;
        cache_size = new_cap;

        /* compute new rows sequentially */
        for (idx_t rr = max_row + 1; rr <= r; ++rr) {
            size_t rr_start = (size_t)(rr - 1) * rr / 2;
            for (idx_t k = 1; k <= rr; ++k) {
                seq_t val = 0;

                /* term T(rr-1, k) if rr-1 >= k */
                if (rr - 1 >= k) {
                    size_t idx_prev = (size_t)(rr - 2) * (rr - 1) / 2 + (k - 1);
                    val += cache[idx_prev];
                }

                /* term T(0, k) = 1 when rr == k */
                if (rr == k) {
                    val += 1;
                }
                /* term T(rr-k, k) when rr-k >= k */
                else if (rr > k && rr - k >= k) {
                    size_t idx_rm = (size_t)(rr - k - 1) * (rr - k) / 2 + (k - 1);
                    val += cache[idx_rm];
                }

                cache[rr_start + (k - 1)] = val;
            }
        }
        max_row = r;
    }

    /* compute column index within the row */
    idx_t col = n - row_start - 1;   /* 0‑based index in row r */
    return cache[row_start + col];
}
