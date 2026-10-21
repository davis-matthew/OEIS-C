/*
 * A292404  p-INVERT of (1,0,0,1,0,0,0,0,0,0,...), where p(S) = 1 - S^4.
 * https://oeis.org/A292404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 4*a(n-7) + 6*a(n-10) + 4*a(n-13) + a(n-16) for
 *            n≥17 a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0, a(6)=4,
 *            a(7)=1, a(8)=0, a(9)=6, a(10)=8, a(11)=1, a(12)=4, a(13)=28,
 *            a(14)=12, a(15)=2, a(16)=56
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 74, u64 n <= 142,
 *            u128 n <= 280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored values up to max_computed */
static idx_t max_computed = (idx_t)-1; /* last index for which cache is defined */

/* Helper to initialise or extend the cache up to index n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate for 17 initial values */
        size_t sz = 17;
        cache = (seq_t*)calloc(sz, sizeof(seq_t));
        static const seq_t init[17] = {
            0, 0, 0, 1, 0, 0, 4, 1, 0, 6, 8, 1, 4, 28, 12, 2, 56
        };
        for (idx_t i = 0; i <= 16; ++i)
            cache[i] = init[i];
        max_computed = 16;
    }

    if (n <= max_computed)
        return;

    size_t old_sz = (size_t)(max_computed + 1);
    size_t new_sz = (size_t)(n + 1);

    seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure */
    cache = tmp;

    /* new entries are initially zero */
    for (idx_t i = old_sz; i <= n; ++i)
        cache[i] = 0;

    /* compute missing terms sequentially */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 4]
                 + (seq_t)4 * cache[i - 7]
                 + (seq_t)6 * cache[i - 10]
                 + (seq_t)4 * cache[i - 13]
                 + cache[i - 16];
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    /* initial values for n < 17 are hard‑coded */
    if (n < 17) {
        static const seq_t init[17] = {
            0, 0, 0, 1, 0, 0, 4, 1, 0, 6, 8, 1, 4, 28, 12, 2, 56
        };
        return init[n];
    }

    /* compute or fetch from cache for n >= 17 */
    ensure_cache(n);
    return cache[n];
}
