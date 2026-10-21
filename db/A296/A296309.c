/*
 * A296309  Number of n X 4 0..1 arrays with each 1 adjacent to 3 or 6
 *          king-move neighboring 1s.
 * https://oeis.org/A296309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A296309
   Recurrence: a(n) = a(n-1) + 4*a(n-3) - a(n-4) - 2*a(n-6)
   Initial values:
   a(1)=1, a(2)=4, a(3)=7, a(4)=10, a(5)=23, a(6)=45
*/

#include <stdlib.h>

static const seq_t base[7] = {0, 1, 4, 7, 10, 23, 45};

seq_t a(idx_t n)
{
    /* Handles n ≥ 1.  For n ≤ 6 we return the constant values. */
    if (n <= 6) {
        return base[(size_t)n];
    }

    /* Persistent cache for memoisation */
    static seq_t *cache = NULL;      /* array of a[1..max_cached] */
    static size_t cache_cap = 0;     /* allocated size (≥ max_cached + 1) */
    static idx_t   cache_max = 0;    /* largest n computed so far */

    /* Ensure the cache is large enough for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base values the first time we need them */
    if (cache_max < 1) {
        for (size_t i = 1; i <= 6; ++i) {
            cache[i] = base[i];
        }
    }

    /* Compute any missing terms up to n */
    idx_t start = (cache_max >= 6) ? cache_max + 1 : 7;   /* start at 7 or next index */
    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i-1];          /* a(i-1) */
        seq_t t2 = cache[i-3] * 4;      /* 4*a(i-3) */
        seq_t t3 = cache[i-4];          /* a(i-4) */
        seq_t t4 = cache[i-6] * 2;      /* 2*a(i-6) */
        cache[i] = t1 + t2 - t3 - t4;
    }
    cache_max = n;

    return cache[n];
}
