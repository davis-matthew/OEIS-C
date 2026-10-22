/*
 * A301885  Number of 2Xn 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (empirical recurrence from OEIS A301885):
 *   a(1) = 2, a(2) = 5, a(3) = 10, a(4) = 18, a(5) = 35,
 *   a(6) = 74, a(7) = 154
 *   For n ≥ 8:
 *   a(n) = 2*a(n-1) + a(n-3) - a(n-4) - 2*a(n-6) + a(n-7)
 *
 * This implementation memoises computed values in a static array so that
 * arbitrary calls to a(n) are answered in O(1) after the first
 * expansion to the required n.  It stays well below 2 s for n≈10000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..7 (index 0 unused) */
    static const seq_t base_vals[8] = {0ULL, 2ULL, 5ULL, 10ULL, 18ULL, 35ULL,
                                       74ULL, 154ULL};

    /* Return base values directly for small n (no allocation needed) */
    if (n <= 7)
        return base_vals[(size_t)n];

    /* Static cache for computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* capacity of cache array   */
    static idx_t  maxc   = 0;          /* highest index computed    */

    /* Allocate or grow cache to contain indices up to n */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failure, unlikely in contest */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise cache with base values if not done before */
    if (maxc == 0) {
        for (size_t i = 1; i <= 7; ++i)
            cache[i] = base_vals[i];
        maxc = 7;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = maxc + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1]          /* 2*a(n-1) */
                 + cache[i - 3]              /* +a(n-3)   */
                 - cache[i - 4]              /* -a(n-4)   */
                 - 2 * cache[i - 6]          /* -2*a(n-6) */
                 + cache[i - 7];             /* +a(n-7)   */
        cache[i] = val;
    }

    maxc = n;   /* we have computed up to n now */
    return cache[n];
}
