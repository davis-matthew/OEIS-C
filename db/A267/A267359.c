/*
 * A267359  Binary representation of the n-th iteration of the "Rule 125"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10001*a(n-2) - 10000*a(n-4) for n > 8 (Initial values
 *            a(0)..a(8) are: 1, 11, 11100, 101111, 111110000, 1000111111,
 *            1111101000000, 10001111111111, 11111010000000000)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy static cache: grows as needed,
       using the recurrence once all initial values are known.   */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */

    /* Allocate initial cache of size 9 when first called */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – improbable */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)11100;
        cache[3] = (seq_t)101111;
        cache[4] = (seq_t)111110000;
        cache[5] = (seq_t)1000111111;
        cache[6] = (seq_t)1111101000000;
        cache[7] = (seq_t)10001111111111;
        cache[8] = (seq_t)11111010000000000;
    }

    /* Return from cache if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache to store up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return 0;           /* allocation failure – improbable */
    cache = new_cache;

    /* Compute missing entries using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i > 8) {
            seq_t term1 = (seq_t)10001 * cache[i - 2];
            seq_t term2 = (seq_t)10000 * cache[i - 4];
            cache[i] = term1 - term2;
        }
        /* indices 0..8 already initialized; skip them */
    }
    cache_len = new_len;

    return cache[n];
}
