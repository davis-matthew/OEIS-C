/*
 * A384468  a(0) = 1; for n >= 1, a(n) = a(n-1)/2 if a(n-1) is even, otherwise
 *          a(n) = 2*a(n-1) + n.
 * https://oeis.org/A384468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n≥1, if a(n-1) even then a(n)=a(n-1)/2 else a(n)=2*a(n-1)+n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialize cache with a(0) if empty */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;            /* out‑of‑memory safeguard */
        cache[0] = 1;
        cache_cap = 1;
        cache_len = 1;
    }

    /* Return known value if already computed */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Grow cache capacity to accommodate up to n */
    size_t target = (size_t)n + 1;
    if (target > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < target) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out‑of‑memory safeguard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n */
    for (size_t idx = cache_len; idx < target; ++idx) {
        seq_t prev = cache[idx - 1];
        if (prev & 1) {                       /* odd case: a(n)=2*a(n-1)+n */
            cache[idx] = (prev << 1) + (seq_t)idx;
        } else {                              /* even case: a(n)=a(n-1)/2 */
            cache[idx] = prev >> 1;
        }
    }

    cache_len = target;
    return cache[n];
}
