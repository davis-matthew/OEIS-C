/*
 * A392582  Expansion of 1 / ((1-x)^4 - x^2)^2.
 * https://oeis.org/A392582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A392582
   Expansion of 1 / ((1-x)^4 - x^2)^2.
   The sequence satisfies the 8th‑order linear recurrence:
   a(n) = 8*a(n-1) - 26*a(n-2) + 48*a(n-3) - 59*a(n-4)
          + 48*a(n-5) - 26*a(n-6) + 8*a(n-7) - a(n-8)
   with initial values
   a(0)=1, a(1)=8, a(2)=38, a(3)=144, a(4)=489,
   a(5)=1568, a(6)=4858, a(7)=14696.
*/

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of previously computed terms */
static size_t cache_len = 0;     /* current size of cache (number of elements) */

/* Ensure the cache contains all terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* No cache yet – create the first eight terms explicitly */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);                /* allocation failed */
        cache[0] = 1;  cache[1] = 8;  cache[2] = 38;  cache[3] = 144;
        cache[4] = 489; cache[5] = 1568; cache[6] = 4858; cache[7] = 14696;
    }

    /* Grow the cache if n exceeds the current length */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len) return;

    /* Reallocate to the larger size */
    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!cache) exit(1);                /* allocation failed */

    /* Compute each new term using the recurrence */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val;
        if (sizeof(seq_t) <= 8) {                 /* seq_t fits in 64‑bit */
            unsigned __int128 p = 8  * (unsigned __int128)cache[i-1];
            p += 48 * (unsigned __int128)cache[i-3];
            p += 48 * (unsigned __int128)cache[i-5];
            p += 8  * (unsigned __int128)cache[i-7];
            unsigned __int128 n = 26 * (unsigned __int128)cache[i-2];
            n += 59 * (unsigned __int128)cache[i-4];
            n += 26 * (unsigned __int128)cache[i-6];
            n += (unsigned __int128)cache[i-8];
            val = (seq_t)(p - n);
        } else {                                 /* arbitrary‑precision seq_t */
            seq_t p = 8  * cache[i-1];
            p += 48 * cache[i-3];
            p += 48 * cache[i-5];
            p += 8  * cache[i-7];
            seq_t n = 26 * cache[i-2];
            n += 59 * cache[i-4];
            n += 26 * cache[i-6];
            n += cache[i-8];
            val = p - n;
        }
        cache[i] = val;
    }
    cache_len = needed;
}

/* Public interface required by the framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
