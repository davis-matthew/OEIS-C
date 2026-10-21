/*
 * A292412  Numbers of the form Fibonacci(2*k-1) or Lucas(2*k-1); i.e., union
 *          of sequences A001519 and A002878.
 * https://oeis.org/A292412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4) for n>4; initial values: a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=4
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;      /* cache[1..cached_max] */
    static idx_t cached_max = 0;     /* highest n currently stored */
    static size_t capacity = 0;      /* allocated size (>= cached_max+1) */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 8;                     /* sufficient for first 4 terms */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure → 0 */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cached_max = 4;
    }

    /* if requested term already computed, return it */
    if (n <= cached_max) return cache[n];

    /* extend cache up to n, computing new terms */
    for (idx_t i = cached_max + 1; i <= n; ++i) {

        /* ensure enough space in the cache array */
        if ( (size_t)(i + 1) > capacity ) {
            size_t new_cap = capacity * 2;
            while (new_cap < (size_t)(i + 1)) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            capacity = new_cap;
        }

        /* recurrence: a(i) = 3*a(i-2) - a(i-4) */
        cache[i] = 3 * cache[i - 2] - cache[i - 4];
    }

    cached_max = n;
    return cache[n];
}
