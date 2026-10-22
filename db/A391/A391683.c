/*
 * A391683  Number of integer compositions of n that are the first sums of
 *          some composition.
 * https://oeis.org/A391683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3,
 *            a(n)=3*a(n-2)+a(n-3)-2*a(n-4)-2*a(n-5) for n>=6 (derived from
 *            the rational generating function)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static bool initialized = false;

    /* Initialise for the first call.  We keep indices 0..5 in the array.
       Index 0 is unused to keep the code simple.  */
    if (!initialized) {
        cache_size = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();           /* out of memory */
        cache[0] = 0;
        cache[1] = 0;  /* a(1) */
        cache[2] = 1;  /* a(2) */
        cache[3] = 1;  /* a(3) */
        cache[4] = 2;  /* a(4) */
        cache[5] = 3;  /* a(5) */
        initialized = true;
    }

    /* Grow cache if necessary.  The array always has indices
       0 .. cache_size-1 inclusive.  */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cache_size = new_size;

        /* Compute missing values from the last known up to n.  */
        for (idx_t i = 6; i <= n; ++i) {
            seq_t term = 0;
            /* a(i) = 3*a(i-2) + a(i-3) - 2*a(i-4) - 2*a(i-5) */
            term = 3 * cache[i - 2];
            term += cache[i - 3];
            term -= 2 * cache[i - 4];
            term -= 2 * cache[i - 5];
            cache[i] = term;
        }
    }

    return cache[n];
}
