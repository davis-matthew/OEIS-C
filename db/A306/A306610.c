/*
 * A306610  a(n) = (2*cos(Pi/15))^(-n) + (2*cos(7*Pi/15))^(-n) +
 *          (2*cos(11*Pi/15))^(-n) + (2*cos(13*Pi/15))^(-n), for n >= 1.
 * https://oeis.org/A306610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)-a(n-3)-a(n-4); Initial values: a(1)=4,
 *            a(2)=24, a(3)=109, a(4)=524
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache array holding a(1) … a(cache_len). */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Grow the cache so that it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    /* First time: allocate space for the first four terms. */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)4 * sizeof(seq_t));
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)109;
        cache[3] = (seq_t)524;
        cache_len = 4;
    }

    /* Extend the cache if the requested index is beyond what we have. */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        /* In practice the allocation will succeed; if not we keep the old
           cache to avoid undefined behaviour. */
        if (new_cache != NULL) {
            cache = new_cache;
        }
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = 4 * cache[i-1] + 4 * cache[i-2]
                       - cache[i-3] - cache[i-4];
        }
        cache_len = n;
    }
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety guard; n>=1 per spec */
    ensure_cache(n);
    return cache[n-1];                    /* indices are 0‑based in cache */
}
