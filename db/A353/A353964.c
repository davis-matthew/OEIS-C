/*
 * A353964  Number of tilings of a 2 X n rectangle using 2 X 2 and 1 X 1 tiles
 *          and right trominoes.
 * https://oeis.org/A353964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + 2*a(n-3) Initial values: a(0)=1,
 *            a(1)=1, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial small indices without allocating */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 6;

    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* Number of elements currently stored */

    /* If the term is already in the cache, return it immediately */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Need to grow the cache up to index n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (cache == NULL) {
        /* Allocation failed – in a contest environment we might abort; */
        /* here we simply return 0 as a fallback */
        return 0;
    }

    /* If this is the first allocation, initialise the base cases */
    if (old_len == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 6;
        old_len = 3;
    }

    /* Compute and store the missing terms up to n */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + 5 * cache[i-2] + 2 * cache[i-3];
    }

    cache_len = new_len;  /* Update the cache length */
    return cache[(size_t)n];
}
