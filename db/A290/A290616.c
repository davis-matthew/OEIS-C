/*
 * A290616  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - 2 S - S^2.
 * https://oeis.org/A290616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6) with
 *            initial values a(0)=2, a(1)=5, a(2)=12, a(3)=31, a(4)=80,
 *            a(5)=205, a(6)=526 (as given in the OEIS entry for A290616)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Helper to grow the cache if needed */
    if (n + 1 > (size_t)n) { /* force compiler to use size_t in calculation */
        if (cache == NULL) {
            /* Initial capacity: at least 64 or n+1 if n is large */
            cache_cap = (n + 1) > 64 ? (n + 1) : 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (cache == NULL) return 0; /* allocation failure, return 0 */
            /* Initialise first 7 terms */
            cache[0] = 2;
            cache[1] = 5;
            cache[2] = 12;
            cache[3] = 31;
            cache[4] = 80;
            cache[5] = 205;
            cache[6] = 526;
            cache_len = 7;
        } else if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap >= (size_t)n + 1 ? cache_cap * 2 : (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure, return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* If requested n is less than current cache length, return it */
        if ((size_t)n < cache_len) {
            return cache[(size_t)n];
        }

        /* Compute missing terms up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Recurrence applies for i >= 7 (i == 6 already initialised) */
            if (i >= 7) {
                seq_t v = 0;
                v += 2 * cache[i - 1];
                v += cache[i - 2];
                v += 2 * cache[i - 3];
                v -= 2 * cache[i - 4];
                v -= cache[i - 6];
                cache[i] = v;
            } else {
                /* For i < 7, values are already set */
            }
        }
        cache_len = (size_t)n + 1;
        return cache[(size_t)n];
    }

    /* If we reach here, n is negative or overflowed; sequence is only defined for n >= 0 */
    return 0;
}
