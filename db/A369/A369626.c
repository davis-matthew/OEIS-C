/*
 * A369626  a(n) is the number of permutations of [n] which avoid the patterns
 *          1234, 1324, and 2413.
 * https://oeis.org/A369626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 14*a(n-3) - 4*a(n-4); Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL; /* static cache for memoisation */
static idx_t   cache_size = 0; /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)21;

    /* ensure cache has space for indices [0 … n] */
    if (cache_size <= n) {
        /* allocate or resize */
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failed */
        /* if expanding, initialise newly added entries to 0 */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise base values if they are not already present */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1; cache[1] = (seq_t)1;
        cache[2] = (seq_t)2; cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
    }

    /* compute missing values up to n */
    for (idx_t i = 5; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed */
        seq_t val = (seq_t)7 * cache[i-1];
        val = val - (seq_t)16 * cache[i-2];
        val = val + (seq_t)14 * cache[i-3];
        val = val - (seq_t)4  * cache[i-4];
        cache[i] = val;
    }

    return cache[n];
}
