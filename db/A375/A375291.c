/*
 * A375291  Expansion of (1 - x + x^4)/((1 - x + x^4)^2 + 4*x^5).
 * https://oeis.org/A375291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - 2*a(n-4) - 2*a(n-5) - a(n-8) a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=0, a(5)=-5, a(6)=-14, a(7)=-27
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Initial values for n = 0..7 */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)1,
        (seq_t)0,  (seq_t)-5, (seq_t)-14, (seq_t)-27
    };

    /* Initialise cache with the initial values if not yet done. */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure; return 0 as fallback */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* If requested n is less than the cached size, return it directly. */
    if (n < cache_cap)
        return cache[n];

    /* Grow the cache to at least n+1 elements. */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure; return 0 as fallback */
    cache = tmp;

    /* Compute values from the first missing index up to n. */
    for (idx_t i = cache_cap; i <= n; ++i) {
        /* For indices < 8 we already have the initial values */
        if (i < 8) {
            cache[i] = init[i];
            continue;
        }

        /* Apply the recurrence:
           a(i) = 2*a(i-1) - a(i-2) - 2*a(i-4) - 2*a(i-5) - a(i-8) */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t8 = cache[i - 8];

        seq_t val = 0;
        val += 2 * t1;
        val -= t2;
        val -= 2 * t4;
        val -= 2 * t5;
        val -= t8;

        cache[i] = val;
    }

    cache_cap = new_cap;
    return cache[n];
}
