/*
 * A285231  Number of entries in the third cycles of all permutations of [n].
 * https://oeis.org/A285231
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(3) = 1. For n >= 3: a(n+1) = (n+2) * a(n) + s(n,2) where
 *            s(n,2) is the unsigned Stirling number of the first kind for n
 *            and 2. s(n,2) satisfies s(3,2) = 3, s(n+1,2) = (n-1)! + n *
 *            s(n,2). The recurrence is derived from the cycle‐decomposition
 *            of permutations and allows the sequence to be genera...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* Static cache of previously computed a(n).  cache[i] = a(i). */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    /* Highest n for which cache[i] is valid. */
    static idx_t lastComputed = 2;
    /* (lastComputed-1)!  and  s(lastComputed,2).  They are only
       needed while extending the cache.  They are valid for
       lastComputed >= 3. */
    static seq_t lastFact = 0;   /* (lastComputed-1)! */
    static seq_t lastS2   = 0;   /* s(lastComputed,2) */

    /* Ensure the cache is large enough to hold index n. */
    if (cacheSize <= (size_t)n) {
        seq_t *tmp = seq_realloc(cache, cacheSize, (size_t)n + 1);
        cache = tmp;
        for (size_t i = cacheSize; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)0;
        }
        cacheSize = (size_t)n + 1;
    }

    /* Initialise the base case a(3)=1 together with necessary
       auxiliary values (fact and s2).  These are only set once. */
    if (lastComputed < 3) {
        lastComputed = 3;          /* we now know a(3) */
        cache[3] = (seq_t)1;       /* a(3) = 1                */
        lastFact   = (seq_t)2;     /* 2! = 2                   */
        lastS2     = (seq_t)3;     /* s(3,2) = 3               */
    }

    /* Extend the cache up to n.  Each step uses the recurrence above. */
    while (lastComputed < n) {
        const idx_t current = lastComputed;
        /* compute s(current+1,2) from s(current,2) and (current-1)! */
        seq_t newS2 = lastFact + ((seq_t)current) * lastS2;
        /* (current)!  =  (current-1)! * current   */
        seq_t newFact = lastFact * ((seq_t)current);
        /* a(current+1)  = (current+2) * a(current) + s(current,2)   */
        seq_t newVal = (((seq_t)(current + 2)) * cache[current]) + lastS2;

        /* store the newly computed value */
        cache[current + 1] = newVal;

        /* update state for the next iteration */
        lastComputed = current + 1;
        lastFact     = newFact;
        lastS2       = newS2;
    }

    return cache[n];
}
