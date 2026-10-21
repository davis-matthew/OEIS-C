/*
 * A292321  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - S^2 - S^3.
 * https://oeis.org/A292321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 4*a(n-3) - a(n-5) - 3*a(n-6) + a(n-9) for n >=
 *            10; Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=4,
 *            a(5)=5, a(6)=7, a(7)=17, a(8)=23, a(9)=38
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 44, u64 n <= 87, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoised values          */
    static size_t cache_size = 0;      /* current allocated size   */
    size_t idx = (size_t)n;            /* index into the array     */

    /* first call – initialise the cache with the 10 known terms   */
    if (cache == NULL) {
        cache_size = 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();           /* out of memory             */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)7;
        cache[7] = (seq_t)17;
        cache[8] = (seq_t)23;
        cache[9] = (seq_t)38;
    }

    /* extend cache if the requested index is beyond the cached region */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory          */
        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i) {
            /* recurrence valid for i >= 10, guaranteed by the loop bounds */
            cache[i] = cache[i-2] + (seq_t)4 * cache[i-3]
                       - cache[i-5] - (seq_t)3 * cache[i-6]
                       + cache[i-9];
        }
        cache_size = new_size;
    }

    return cache[idx];
}
