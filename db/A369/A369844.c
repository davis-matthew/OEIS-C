/*
 * A369844  Number of compositions of 5*n-4 into parts 2 and 5.
 * https://oeis.org/A369844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) with
 *            a(1)=0, a(2)=1, a(3)=4, a(4)=11, a(5)=29
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;          /* array of terms, index 0 unused */
    static size_t cache_size = 0;        /* size of allocated array */

    if (n < 1) return 0;                 /* undefined for n < 1 */

    size_t idx = (size_t)n;

    /* enlarge cache if necessary */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;       /* we need indices 0..idx inclusive */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failure – return 0 or handle error appropriately */
            return 0;
        }
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        else {
            memset(new_cache, 0, new_size * sizeof(seq_t));
        }
        cache = new_cache;
        cache_size = new_size;

        /* initialise the first five terms if they exist in the cache */
        if (cache_size > 1) cache[1] = 0;
        if (cache_size > 2) cache[2] = 1;
        if (cache_size > 3) cache[3] = 4;
        if (cache_size > 4) cache[4] = 11;
        if (cache_size > 5) cache[5] = 29;
    }

    /* if the value has already been computed, return it */
    if (cache[idx] != 0 || idx < 6)
        return cache[idx];

    /* compute missing terms up to idx */
    for (size_t i = 6; i <= idx; ++i) {
        if (cache[i] != 0) continue; /* should not happen, but guard against double work */
        cache[i] = ((seq_t)5 * cache[i-1])
                 - ((seq_t)9 * cache[i-2])
                 + ((seq_t)10 * cache[i-3])
                 - ((seq_t)5 * cache[i-4])
                 + cache[i-5];
    }

    return cache[idx];
}
