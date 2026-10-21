/*
 * A253413  Number of n-bit legal circular binary words with maximal set of
 *          1's.
 * https://oeis.org/A253413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6) for n>7 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=5, a(6)=5, a(7)=14
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 65, u64 n <= 131,
 *            u128 n <= 262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of values stored */

    /* initialise cache with known values for n = 0..7 */
    if (cache == NULL) {
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        size  = 8;
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 6;
        cache[5] = 5;
        cache[6] = 5;
        cache[7] = 14;
    }

    /* extend cache if necessary */
    if ((size_t)n >= size) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }
        /* compute new values using the recurrence */
        for (size_t i = size; i <= (size_t)n; ++i) {
            if (i >= 8) {
                cache[i] = cache[i-2] + cache[i-3] + cache[i-4] - cache[i-6];
            }
        }
        size = needed;
    }

    return cache[(size_t)n];
}
