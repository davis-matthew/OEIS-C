/*
 * A369845  Number of compositions of 5*n into parts 3 and 5.
 * https://oeis.org/A369845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 11*a(n-3) - 5*a(n-4) + a(n-5),
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=7. This recurrence is
 *            derived from the generating function (1-x)^4/((1-x)^5 - x^3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to handle arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Handle the first five terms without allocation */
    if (n < 5) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)2;
            case 4: return (seq_t)7;
        }
    }

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)7;
        cache_size = 5;
    }

    /* Grow cache to accommodate n */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* All terms with i < 5 are already set */
            if (i < 5) continue;
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            /* Compute: 5*t1 - 10*t2 + 11*t3 - 5*t4 + t5 */
            seq_t val = 5 * t1;
            val += 11 * t3;
            val += t5;
            val -= 10 * t2;
            val -= 5 * t4;
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
}
