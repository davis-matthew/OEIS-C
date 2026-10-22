/*
 * A362126  Expansion of 1/(1 - x*(1+x)^2)^2.
 * https://oeis.org/A362126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) -
 *            a(n-6) for n > 5, with initial values a(0)=1, a(1)=2, a(2)=7,
 *            a(3)=18, a(4)=47, a(5)=118
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t INIT[6] = { (seq_t)1, (seq_t)2, (seq_t)7,
                               (seq_t)18, (seq_t)47, (seq_t)118 };

static seq_t *cache = NULL;      /* dynamic cache of computed terms */
static size_t cache_len = 0;     /* number of elements stored in cache */

/* Computes a(n) from A362126.  Caching allows arbitrary order of calls. */
seq_t a(idx_t n)
{
    /* Handle the first 6 terms directly. */
    if (n < 6) {
        return INIT[(size_t)n];
    }

    /* If cache is uninitialized, allocate space up to max(n,5)+1 */
    if (cache_len == 0) {
        size_t init_len = (size_t)n + 1;
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) { /* allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        for (size_t i = 0; i < 6; ++i) {
            cache[i] = INIT[i];
        }
        cache_len = 6;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            return (seq_t)0; /* allocation failed */
        }
        cache = new_cache;
        /* Compute new terms using recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t term1 = 2 * cache[i - 1] + 3 * cache[i - 2];
            seq_t term2 = 2 * cache[i - 3] + 6 * cache[i - 4]
                        + 4 * cache[i - 5] + cache[i - 6];
            cache[i] = term1 - term2;   /* term1 > term2 for all n ≥ 6 */
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
