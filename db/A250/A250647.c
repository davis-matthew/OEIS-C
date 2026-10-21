/*
 * A250647  Number of length 3+1 0..n arrays with the sum of the maximum of
 *          each adjacent pair multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - 3*a(n-4) + 3*a(n-5) +
 *            a(n-6) - a(n-7) with initial values: a(1)=6, a(2)=23, a(3)=44,
 *            a(4)=89, a(5)=134, a(6)=219, a(7)=296 (valid for n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    // array holding computed a(1)..a(k)
static size_t cache_len = 0;   // number of values stored in cache

// initialize the cache with the first 7 terms
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_len = 7;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
    if (cache == NULL)
        abort(); /* out of memory */
    cache[0] = 6;    // a(1)
    cache[1] = 23;   // a(2)
    cache[2] = 44;   // a(3)
    cache[3] = 89;   // a(4)
    cache[4] = 134;  // a(5)
    cache[5] = 219;  // a(6)
    cache[6] = 296;  // a(7)
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, return 0 for safety */
    // ensure cache is initialized
    init_cache();

    size_t target = (size_t)n; /* working copy in size_t (n ≤ 10000 in tests) */
    if (target > cache_len) {
        /* extend cache to hold values up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * target);
        if (new_cache == NULL)
            abort(); /* out of memory */
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i < target; ++i) {
            /* indices: a(i+1) depends on a(i)..a(i-6) (1‑based) */
            seq_t pos = cache[i-1]                          /* a(i)   */
                      + 3 * cache[i-2]                    /* +3*a(i-1) */
                      + 3 * cache[i-5]                    /* +3*a(i-4) */
                      +      cache[i-6];                  /* +a(i-5) */
            seq_t neg = 3 * cache[i-3]                      /* -3*a(i-2) */
                      + 3 * cache[i-4]                      /* -3*a(i-3) */
                      +      cache[i-7];                    /* -a(i-6) */
            cache[i] = pos - neg;     /* pos >= neg, safe for unsigned */
        }
        cache_len = target;
    }
    return cache[target - 1]; /* cache[0] is a(1) */
}
