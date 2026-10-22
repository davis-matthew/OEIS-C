/*
 * A301285  Coordination sequence for node of type V2 in "car" 2-D tiling (or
 *          net).
 * https://oeis.org/A301285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + 3*a(n-3) - 2*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) + a(n-7) - a(n-8) for n > 8 Initial values: a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=10, a(4)=12, a(5)=14, a(6)=20, a(7)=23,
 *            a(8)=22
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (any call order) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms stored */

    /* initialise cache with the first 9 terms once */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 9);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)3;  /* a(1) */
        cache[2] = (seq_t)6;  /* a(2) */
        cache[3] = (seq_t)10; /* a(3) */
        cache[4] = (seq_t)12; /* a(4) */
        cache[5] = (seq_t)14; /* a(5) */
        cache[6] = (seq_t)20; /* a(6) */
        cache[7] = (seq_t)23; /* a(7) */
        cache[8] = (seq_t)22; /* a(8) */
        cache_len = 9;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* extend cache up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL)
        return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = cache[(size_t)(i - 1)];
        seq_t t2 = cache[(size_t)(i - 2)];
        seq_t t3 = cache[(size_t)(i - 3)];
        seq_t t4 = cache[(size_t)(i - 4)];
        seq_t t5 = cache[(size_t)(i - 5)];
        seq_t t6 = cache[(size_t)(i - 6)];
        seq_t t7 = cache[(size_t)(i - 7)];
        seq_t t8 = cache[(size_t)(i - 8)];

        seq_t value =  t1
                     - (t2 + t2)
                     + (t3 + t3 + t3)
                     - (t4 + t4)
                     + (t5 + t5 + t5)
                     - (t6 + t6)
                     +  t7
                     -  t8;

        cache[(size_t)i] = value;
    }
    cache_len = new_len;

    return cache[(size_t)n];
}
