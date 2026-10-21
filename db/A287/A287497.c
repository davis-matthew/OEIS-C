/*
 * A287497  Number of maximal matchings in the n-web graph.
 * https://oeis.org/A287497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+4*a(n-2)+3*a(n-3)-2*a(n-4)+a(n-6); Initial terms:
 *            a(1)=1, a(2)=9, a(3)=22, a(4)=53, a(5)=166, a(6)=432
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for the sequence values.  The array is zero‑based,
   storing a(1) in cache[0], a(2) in cache[1], etc. */
static seq_t *cache = NULL;          /* pointer to the allocated array   */
static size_t cache_size = 0;        /* number of values stored (>=0)    */
static size_t cache_cap = 0;         /* capacity of the allocated array  */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 only.  Below that, return 0. */
    if (n < 1) return 0;

    /* If the value is already cached, return it. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Ensure we have enough capacity in the cache. */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failure: terminate with error, safe for tests */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill the cache from the first missing index up to n. */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        seq_t val;
        switch (k) {
            case 1: val = 1;  break;
            case 2: val = 9;  break;
            case 3: val = 22; break;
            case 4: val = 53; break;
            case 5: val = 166;break;
            case 6: val = 432;break;
            default: {
                /* a(k) = a(k-1)+4*a(k-2)+3*a(k-3)-2*a(k-4)+a(k-6) */
                val = cache[k - 2]          /* a(k-1) */
                    + (seq_t)4 * cache[k - 3]   /* a(k-2) */
                    + (seq_t)3 * cache[k - 4]   /* a(k-3) */
                    - (seq_t)2 * cache[k - 5]   /* a(k-4) */
                    + cache[k - 7];            /* a(k-6) */
            } break;
        }
        cache[k - 1] = val;
    }

    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
