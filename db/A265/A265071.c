/*
 * A265071  Coordination sequence for (3,3,4) tiling of hyperbolic plane.
 * https://oeis.org/A265071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-6) for n>6 a(0)=1, a(1)=3,
 *            a(2)=6, a(3)=10, a(4)=15, a(5)=22, a(6)=31
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 61, u64 n <= 127,
 *            u128 n <= 258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached values of the sequence */
static idx_t  cache_size = 0;      /* allocated size of cache      */
static idx_t  computed_up_to = 0;  /* highest index already computed */

/* initialise the cache with the first 7 terms, if not already done */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_size = 8;                     /* at least 0..6 plus room */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    }
    if (computed_up_to < 6) {
        static const seq_t init_vals[7] = { 1, 3, 6, 10, 15, 22, 31 };
        for (idx_t i = 0; i <= 6; ++i)
            cache[i] = init_vals[i];
        computed_up_to = 6;
    }
}

/* ensure that the cache can hold indices up to n */
static void ensure_size(idx_t n)
{
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }
}

/* return a(n) for any n >= 0, using memoisation */
seq_t a(idx_t n)
{
    init_cache();        /* make sure base terms exist */

    if (n <= 6)
        return cache[n];

    ensure_size(n);

    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = cache[i-2] + cache[i-3] + cache[i-4] - cache[i-6];
        }
        computed_up_to = n;
    }

    return cache[n];
}
