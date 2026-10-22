/*
 * A359886  Number of 3-dimensional tilings of a 2 X 2 X n box using 2 X 2 X 1
 *          plates and trominos (three 1 X 1 X 1 cubes).
 * https://oeis.org/A359886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=1, a(2)=3, a(3)=49, a(4)=231, a(5)=789
   a(n) = 2*a(n-1) + a(n-2) + 58*a(n-3) + 72*a(n-4) + 32*a(n-5) - 128*a(n-6)
   for n >= 6
*/
#include <stdlib.h>

/* memoized cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  computed_max = 0;   /* highest index that has been computed */

/* make sure cache has space for indices 0..n and values up to n are computed */
static void ensure_cache(idx_t n)
{
    /* initialise the cache on first use */
    if (cache == NULL) {
        /* at least capacity for 6 base values */
        cache_cap = (size_t)((n + 1) > 6 ? (n + 1) : 6);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();               /* no memory */

        /* base values a(0) .. a(5) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)49;
        cache[4] = (seq_t)231;
        cache[5] = (seq_t)789;
        computed_max = 5;
    }

    /* grow array if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* recurrence 2 applies for i >= 6 */
            seq_t v = 0;
            if (i >= 6) {
                v = 2 * cache[i - 1] + cache[i - 2] +
                    58 * cache[i - 3] + 72 * cache[i - 4] +
                    32 * cache[i - 5] - 128 * cache[i - 6];
                /* store result */
                cache[i] = v;
            } else {
                /* for i < 6 the values are already set */;
            }
        }
        computed_max = n;
    }
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
