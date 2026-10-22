/*
 * A302706  a(n) is the maximum remainder of x^2 + y^2 divided by x + y with 0
 *          < x <= y <= n.
 * https://oeis.org/A302706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{1 <= x <= y <= n} (x^2 + y^2) mod (x + y)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size (indices 0 .. cache_cap-1) */
static idx_t cache_filled = 0;/* highest computed index */

/* Compute a(n), expanding cache as necessary */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but harmless */

    /* Grow underlying array if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out‑of‑memory: give up */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* initialise new entries to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing entries */
    for (idx_t k = cache_filled + 1; k <= n; ++k) {
        seq_t max_res = (k == 1) ? (seq_t)0 : cache[k - 1];

        idx_t y = k;
        idx_t y2 = y * y;   /* no overflow for any build that uses a(n) */

        for (idx_t x = 1; x <= k; ++x) {
            idx_t sum = x + y;
            idx_t rem = (x * x + y2) % sum;
            if ((seq_t)rem > max_res)
                max_res = rem;
        }
        cache[k] = max_res;
    }
    cache_filled = n;

    return cache[n];
}
