/*
 * A314740  Coordination sequence Gal.5.116.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n > 10;
 *            a(0) = 1, a(1) = 5, a(2) = 9, a(3) = 13, a(4) = 18, a(5) = 24,
 *            a(6) = 30, a(7) = 35, a(8) = 39, a(9) = 43, a(10) = 48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the nth term of OEIS sequence A314740.  It
   lazily builds a cache of computed terms so that repeated calls are
   efficient.  The recurrence can be applied for any n >= 11. */
seq_t
a(idx_t n)
{
    /* Initial terms array */
    static const seq_t init[11] = {
        1ULL, 5ULL, 9ULL, 13ULL, 18ULL, 24ULL,
        30ULL, 35ULL, 39ULL, 43ULL, 48ULL
    };

    /* Static cache */
    static seq_t *cache = NULL;    /* pointer to stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms actually stored */

    /* Ensure cache can hold terms up to n */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) exit(1);          /* terminate on allocation failure */
        cache = newptr;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i < 11) {
            cache[i] = init[(size_t)i];
        } else {
            seq_t v = 0;
            v += 2 * cache[i - 1];
            v -= 2 * cache[i - 2];
            v += 2 * cache[i - 3];
            v -= 2 * cache[i - 4];
            v += 2 * cache[i - 5];
            v -= 2 * cache[i - 6];
            v += 2 * cache[i - 7];
            v -= 2 * cache[i - 8];
            v += 2 * cache[i - 9];
            v -=  cache[i -10];
            cache[i] = v;
        }
    }
    cache_len = needed;

    return cache[(size_t)n];
}
