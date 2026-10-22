/*
 * A309402  Number T(n,k) of nonempty subsets of [n] whose element sum is
 *          divisible by k; triangle T(n,k), n >= 1, 1 <= k <= n*(n+1)/2, read
 *          by rows.
 * https://oeis.org/A309402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = T(row, col) of triangle T(n,k) where T(n,k) is * the
 *            number of non‑empty subsets of [n] whose element sum is *
 *            divisible by k. Row and column are determined by the index in *
 *            the OEIS triangle read by rows
 * Range:     u8 n <= 120, u16 n <= 816, u32 n <= 5984, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   compute_row_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
 * 1. Locate the row r and column c for the requested index n.
 * 2. If T(r,*) has not been computed before, compute all T(r,k) for
 *    1 <= k <= r(r+1)/2 by dynamic programming over subset sums.
 * 3. Return the cached value T(r,c).  Results are memoised per row.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/compute_row_T.h"

/* Memoisation storage for computed rows. */
static seq_t **row_cache = NULL;          /* array of pointers to rows */
static size_t cache_capacity = 0;         /* size of row_cache array */

/* Ensure the cache can store rows up to at least 'max_row'. */
static void ensure_row_cache(size_t max_row) {
    if (max_row < cache_capacity)
        return;
    size_t new_cap = max_row + 1;               /* 1‑based indexing */
    row_cache = (seq_t **)realloc(row_cache, new_cap * sizeof(seq_t *));
    for (size_t i = cache_capacity; i < new_cap; ++i)
        row_cache[i] = NULL;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    /* Find the row r and column c corresponding to the n‑th term. */
    idx_t cum = 0;    /* cumulative terms up to previous row */
    idx_t r = 1;
    while (1) {
        idx_t row_terms = (idx_t)r * ((idx_t)r + 1) / 2;
        if (cum + row_terms >= n)
            break;
        cum += row_terms;
        ++r;
    }
    idx_t c = n - cum; /* 1‑based column within the row */

    /* Ensure the row is cached. */
    ensure_row_cache((size_t)r);
    if (row_cache[(size_t)r] == NULL) {
        row_cache[(size_t)r] = compute_row_T((size_t)r);
    }

    return row_cache[(size_t)r][(size_t)c];
}
