/*
 * A333503  Differences between consecutive odd abundant numbers.
 * https://oeis.org/A333503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005231(n+1) – A005231(n) where A005231 are the odd
 *            abundant numbers. The function generates odd abundant numbers up
 *            to the required limit on demand and caches them so that
 *            subsequent calls are O(1). Its running time is essentially the
 *            sieve of sigma for odd numbers up to the required limi...
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* CACHE ------------------------------------------------------------------ */
/* Cache of the odd abundant numbers (stored in uint64_t to keep σ
   computations fast).  The cache is grown only when a larger limit is
   required. */
static uint64_t *odd_abundant_cache = NULL;   /* array of odd abundant numbers */
static idx_t     cache_size = 0;               /* number of stored numbers   */
static idx_t     last_recomputed_limit = 0;    /* limit that was last processed */

/* BUILD ------------------------------------------------------------------- */
/* Compute all odd abundant numbers up to 'limit' (inclusive) and store
   them in odd_abundant_cache.  The routine rebuilds the cache from
   scratch; previous contents are freed. */
static void recompute_one_limit(idx_t limit)
{
    /* Number of odd numbers ≤ limit: (limit/2)+1. */
    size_t half = (size_t)(limit / 2 + 1);

    /* Allocate array for σ of odd numbers. */
    uint64_t *sigma = (uint64_t *)calloc(half, sizeof(uint64_t));
    if (!sigma) return;            /* allocation failure – leave cache as is */

    /* Sieve σ for odd numbers only: for each odd divisor d,
       add d to all odd multiples of d. */
    for (idx_t d = 1; d <= limit; d += 2) {
        for (idx_t m = d; m <= limit; m += d * 2) {
            sigma[(size_t)((m - 1) >> 1)] += d;
        }
    }

    /* Count odd abundant numbers to pre‑allocate the cache. */
    idx_t count = 0;
    for (idx_t m = 3; m <= limit; m += 2) {
        if (sigma[(size_t)((m - 1) >> 1)] > 2ULL * m) {
            count++;
        }
    }

    /* Allocate new cache. */
    uint64_t *new_arr = (uint64_t *)calloc((size_t)count, sizeof(uint64_t));
    if (!new_arr) {
        free(sigma);
        return;                     /* allocation failure – leave cache as is */
    }

    /* Fill the new cache with odd abundant numbers. */
    idx_t pos = 0;
    for (idx_t m = 3; m <= limit; m += 2) {
        if (sigma[(size_t)((m - 1) >> 1)] > 2ULL * m) {
            new_arr[pos++] = (uint64_t)m;
        }
    }

    /* Replace old cache. */
    free(odd_abundant_cache);
    odd_abundant_cache = new_arr;
    cache_size         = count;
    last_recomputed_limit = limit;

    free(sigma);
}

/* ENSURE ------------------------------------------------------------------ */
/* Ensure that at least (n+1) odd abundant numbers are cached.  The
   progression 945 + 1890*k provides an infinite subsequence of odd
   abundant numbers; using 945 + 1890*n as an upper bound guarantees
   at least n+1 numbers. */
static void ensure_odd_abundant(idx_t n)
{
    /* Required limit to obtain n+1 odd abundant numbers. */
    idx_t required_limit = 945ULL + 1890ULL * n;

    if (required_limit > last_recomputed_limit) {
        recompute_one_limit(required_limit);
    }
}

/* MAIN SEQUENCE FUNCTION --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Make sure the cache contains at least (n+1) odd abundant numbers. */
    ensure_odd_abundant(n);

    /* Compute the difference between consecutive odd abundant numbers. */
    uint64_t diff = odd_abundant_cache[n] - odd_abundant_cache[n - 1];
    return (seq_t)diff;
}
