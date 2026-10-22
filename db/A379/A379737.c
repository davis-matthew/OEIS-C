/*
 * A379737  Triangle read by rows where, for n >= k, T(n,k) is the number of
 *          factorizations of n into distinct factors > 1 with sum k.
 * https://oeis.org/A379737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,k) where T(r,k) is the number of
 * factorizations of r into distinct factors > 1 with sum k.
 * a(n) is the triangle read by rows: row r contains T(r,1),…,T(r,r).
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Cache for precomputed rows: row_cache[r] -> array of size r+1
 * where counts[k] = T(r,k).  Index 0 is unused. */
static seq_t **row_cache = NULL;
static size_t cache_rows = 0;

/* Recursive enumeration of factorizations of target.
 * prod: current product, start: lower bound for next factor,
 * sum: current sum, target: number to factor,
 * counts: array for storing the counts by sum. */
static void factor_rec(uint64_t prod, uint64_t start, uint64_t sum,
                       uint64_t target, seq_t *counts)
{
    if (prod == target) {
        counts[sum] += (seq_t)1;
        return;
    }
    if (prod > target)
        return;

    for (uint64_t f = start; f <= target / prod; ++f) {
        uint64_t new_prod = prod * f;
        if (target % new_prod != 0)
            continue;
        factor_rec(new_prod, f + 1, sum + f, target, counts);
    }
}

/* Ensure row_cache contains counts for row r. */
static seq_t *get_row_counts(uint64_t r)
{
    if (r == 0)
        return NULL;

    if (r >= cache_rows) {
        size_t new_size = r + 1;
        seq_t **new_cache = (seq_t**)realloc(row_cache,
                                             new_size * sizeof(seq_t*));
        if (!new_cache)
            return NULL;
        /* zero new pointers */
        memset(new_cache + cache_rows, 0,
               (new_size - cache_rows) * sizeof(seq_t*));
        row_cache = new_cache;
        cache_rows = new_size;
    }

    if (row_cache[r] != NULL)
        return row_cache[r];

    /* Allocate and zero-initialize the counts array. */
    seq_t *counts = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!counts)
        return NULL;
    for (uint64_t i = 0; i <= r; ++i)
        counts[i] = (seq_t)0;

    factor_rec(1, 2, 0, r, counts);
    row_cache[r] = counts;
    return counts;
}

/* Find the row r such that r*(r+1)/2 >= n. */
static uint64_t find_row(idx_t n)
{
    uint64_t lo = 1, hi = 2;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;

    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Determine row r and position k within that row. */
    uint64_t r = find_row(n);
    uint64_t start_index = 1 + (unsigned __int128)(r - 1) * r / 2;
    uint64_t k = n - start_index + 1;

    if (k == 0 || k > r)
        return (seq_t)0;

    seq_t *counts = get_row_counts(r);
    if (!counts)
        return (seq_t)0;
    return counts[k];
}
