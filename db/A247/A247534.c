/*
 * A247534  Number of length 2+3 0..n arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
 *            with initial values: a(1) = 8, a(2) = 45, a(3) = 136, a(4) =
 *            317, a(5) = 600, a(6) = 1033
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[7] = {0ULL, 8ULL, 45ULL, 136ULL, 317ULL, 600ULL, 1033ULL};

static seq_t *cache = NULL;        /* array of computed terms, indices 0..last */
static idx_t cache_last = 0;       /* highest index computed so far */
static idx_t cache_cap  = 0;       /* allocated capacity of cache */

/* Ensure that terms up to n inclusive are ready in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_last) return;

    idx_t needed = n + 1;
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = base_vals[i];
        } else {
            /* compute using the recurrence without negative intermediate values:
               a(i) = (2*a(i-1) + a(i-2) + a(i-4) + 2*a(i-5))
                      - (4*a(i-3) + a(i-6)) */
            seq_t pos = (seq_t)2 * cache[i-1] + cache[i-2]
                      + cache[i-4] + (seq_t)2 * cache[i-5];
            seq_t neg = (seq_t)4 * cache[i-3] + cache[i-6];
            cache[i] = pos - neg;
        }
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}
