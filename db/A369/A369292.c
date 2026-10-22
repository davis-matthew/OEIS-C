/*
 * A369292  Array read by downward antidiagonals: A(n,k) = -A(n-1,k) +
 *          (k+1)*A(n-1,k+1) + A(n-1,k+2) with A(0,k) = 1, n >= 0, k >= 0.
 * https://oeis.org/A369292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   recompute_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369292: A(n,k) = -A(n-1,k) + (k+1)*A(n-1,k+1) + A(n-1,k+2),
 * with A(0,k) = 1.  The sequence a(n) is obtained by reading A(n,k)
 * along downward antidiagonals. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include "../utils/recompute_cache.h"

/* Find the diagonal index d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
static idx_t find_diagonal(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 8589934591ULL;          /* 2^33-1, larger than any d for uint64_t n */
    idx_t ans  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= ( __int128)n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Public function - external linkage. */
seq_t a(idx_t n)
{
    static idx_t cached_max_s = 0;     /* largest d that has been cached */
    static seq_t *cache      = NULL;  /* pointer to triangular table array  */
    static size_t cache_width = 0;    /* width of each row in cache          */

    /* Locate the diagonal d containing n. */
    idx_t d = find_diagonal(n);

    /* Grow cache if necessary. */
    if (!cache || d > cached_max_s) {
        recompute_cache(d, &cache, &cache_width);
        cached_max_s = d;
    }

    /* Indices within the triangular table. */
    idx_t t   = d * (d + 1) / 2;          /* T(d) */
    idx_t p   = n - t;                    /* row index */
    idx_t col = d - p;                    /* column index */

    return cache[p * cache_width + col];
}
