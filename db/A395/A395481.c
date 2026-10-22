/*
 * A395481  Number of derangements of [n] with 2 descents.
 * https://oeis.org/A395481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=4, a(5)=24,
 *            a(n)=7*a(n-1)-16*a(n-2)+12*a(n-3) for n > 5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* highest computed index */

    /* initialise cache with the first 6 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)24;
        max_n = 5;
    }

    /* if the requested n is already cached, return it */
    if (n <= max_n)
        return cache[n];

    /* extend the cache up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)7 * cache[i - 1];
        seq_t term2 = (seq_t)12 * cache[i - 3];
        seq_t sum   = term1 + term2;
        seq_t term3 = (seq_t)16 * cache[i - 2];
        cache[i] = sum - term3;
    }

    max_n = n;
    return cache[n];
}
