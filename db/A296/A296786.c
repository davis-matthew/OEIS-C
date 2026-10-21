/*
 * A296786  a(1) = a(2) = a(5) = 2, a(3) = 1, a(4) = 3, a(6) = 5; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 6.
 * https://oeis.org/A296786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n > 14, with a(1)=2, a(2)=2,
 *            a(3)=1, a(4)=3, a(5)=2, a(6)=5, a(7)=7, a(8)=8, a(9)=7, a(10)=4,
 *            a(11)=11, a(12)=12, a(13)=11, a(14)=4
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t small_vals[15] = {
    0,  // unused 0‑th element
    2,  // a(1)
    2,  // a(2)
    1,  // a(3)
    3,  // a(4)
    2,  // a(5)
    5,  // a(6)
    7,  // a(7)
    8,  // a(8)
    7,  // a(9)
    4,  // a(10)
    11, // a(11)
    12, // a(12)
    11, // a(13)
    4   // a(14)
};

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of allocated elements */
    static idx_t  cache_size = 0;  /* highest computed index */

    if (n < 1) return (seq_t)0; /* undefined, but keep safe */

    /* Ensure cache can hold index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = small_vals[i];
        } else {
            cache[i] = (seq_t)2 * cache[i-4] - cache[i-8];
        }
    }
    cache_size = n;

    return cache[n];
}
