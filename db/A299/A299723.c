/*
 * A299723  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A299723: Number of n×3 0..1 arrays with each element equal to the
 *  number of king‑move adjacent elements, upper left element zero.
 *
 *  Recurrence (OEIS):  a(n) = 6*a(n-1) + 8*a(n-2) + 20*a(n-3) +
 *                       25*a(n-4) + 11*a(n-5) + 2*a(n-6)  for n > 7
 *
 *  Initial values: a(1)=1, a(2)=18, a(3)=141, a(4)=1027,
 *                  a(5)=7664, a(6)=57471, a(7)=430401
 *
 *  The function caches previously computed values, so it works in O(n)
 *  time for any order of calls with n ≥ 1.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types defined by the build */
#ifndef seq_t
/* seq_t is supplied by the build system; it can be a 64‑bit integer,
   an arbitrary precision integer, or a big‑integer class.
   The code below only uses +,-,*,/ and assignments, which are
   guaranteed to be available for seq_t. */
#endif

seq_t a(idx_t n)
{
    /* static cache (1‑indexed) */
    static seq_t *cache = NULL;   /* array of values */
    static size_t cap = 0;        /* current allocated size (cache[1..cap]) */

    if (n == 0) {
        /* The sequence is defined for n≥1; return 0 for safety. */
        return (seq_t)0;
    }

    /* Expand the cache if necessary */
    if ((size_t)n > cap) {
        size_t newCap = (size_t)n + 1;          /* allocate indices 0..n */
        seq_t *newCache = (seq_t *)malloc(newCap * sizeof(seq_t));
        if (!newCache) {
            /* Allocation failed – undefined behaviour in this context. */
            abort();
        }

        /* Copy old values (if any) */
        if (cache) {
            for (size_t i = 1; i <= cap; ++i) {
                newCache[i] = cache[i];
            }
        }

        /* Set initial values for indices 1..7 if not already set */
        static const seq_t init[8] = {0, 1, 18, 141, 1027, 7664, 57471,
                                      430401};
        size_t initLimit = (newCap > 7 ? 7 : newCap - 1);
        for (size_t i = 1; i <= initLimit; ++i) {
            newCache[i] = init[i];
        }

        /* Compute further terms using the recurrence */
        for (size_t i = (cap > 7 ? cap : 7) + 1; i <= (size_t)n; ++i) {
            seq_t val = 0;
            val += (seq_t)6  * newCache[i - 1];
            val += (seq_t)8  * newCache[i - 2];
            val += (seq_t)20 * newCache[i - 3];
            val += (seq_t)25 * newCache[i - 4];
            val += (seq_t)11 * newCache[i - 5];
            val += (seq_t)2  * newCache[i - 6];
            newCache[i] = val;
        }

        /* Free old cache and replace */
        if (cache) {
            free(cache);
        }
        cache = newCache;
        cap   = (size_t)n;
    }

    return cache[n];
}
