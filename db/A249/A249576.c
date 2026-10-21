/*
 * A249576  List of triples (r,s,t): the matrix M = [[1,4,4][1,3,2][1,2,1]] is
 *          raised to successive powers, then (r,s,t) are the square roots of
 *          M[3,1], M[1,1], M[1,3] respectively.
 * https://oeis.org/A249576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*a(n-3)+a(n-6) for n >= 6, a(0) = 0, a(1) = 1, a(2) = 0,
 *            a(3) = 1, a(4) = 1, a(5) = 2
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 78, u64 n <= 154,
 *            u128 n <= 305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static idx_t cache_len   = 6;   /* number of values currently stored */

    /* initialise cache once */
    if (cache == NULL) {
        cache_size = 64;                          /* initial allocation */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)2;
    }

    /* value already in cache */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if needed */
    if (n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= n)
            new_size <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)(2 * cache[i - 3] + cache[i - 6]);
    }
    cache_len = n + 1;   /* we now have values up to n */

    return cache[n];
}
