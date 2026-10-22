/*
 * A396814  Upper (1/2,1) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A396814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4)
 *            - a(n-5) - a(n-6) + a(n-7),    (n ≥ 7)
 * with a(0)=0, a(1)=1, a(2)=2, a(3)=5, a(4)=6,
 * a(5)=11, a(6)=14.
 */
#include <stdlib.h>

static const seq_t init_vals[7] = {0, 1, 2, 5, 6, 11, 14};

seq_t a(idx_t n)
{
    /* Return early if we are within the pre‑computed range. */
    if (n < 7)
        return init_vals[n];

    /* Lazily allocated cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialise cache on the first call. */
    if (cache_len == 0) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_len = 7;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if (n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (cache_cap < need) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;
            while (new_cap < need)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* Compute any missing terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i)
            cache[i] = cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4]
                     - cache[i-5] - cache[i-6] + cache[i-7];

        cache_len = n + 1;
    }

    return cache[n];
}
