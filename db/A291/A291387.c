/*
 * A291387  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - 4 S)^2.
 * https://oeis.org/A291387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) - 32*a(n-3) - 16*a(n-4) (n ≥ 5);
 *            initial values: a(0)=8, a(1)=56, a(2)=352, a(3)=2096, a(4)=12032
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;           /* current size of cache (elements) */
    static idx_t   highest = 0;          /* highest index computed so far */

    /* grow cache if necessary  */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base values if not yet done for the current size  */
    if (highest < 1) {
        cache[0] = (seq_t)8;
        if (cap > 1)  cache[1] = (seq_t)56;
        if (cap > 2)  cache[2] = (seq_t)352;
        if (cap > 3)  cache[3] = (seq_t)2096;
        if (cap > 4)  cache[4] = (seq_t)12032;
        highest = (cap - 1 < 4) ? (idx_t)(cap - 1) : (idx_t)4;
    } else if (highest < 4 && cap > highest) {
        /* fill any missing base terms after an expansion */
        if (highest < 1 && cap > 1)  cache[1] = (seq_t)56;
        if (highest < 2 && cap > 2)  cache[2] = (seq_t)352;
        if (highest < 3 && cap > 3)  cache[3] = (seq_t)2096;
        if (highest < 4 && cap > 4)  cache[4] = (seq_t)12032;
        highest = (cap - 1 < 4) ? (idx_t)(cap - 1) : (idx_t)4;
    }

    /* compute missing terms up to n */
    if (n > highest) {
        for (idx_t i = highest + 1; i <= n; ++i) {
            /* recurrence for i ≥ 5; base terms already set for i ≤ 4 */
            if (i > 4) {
                cache[i] = (seq_t)8  * cache[i-1]
                         - (seq_t)8  * cache[i-2]
                         - (seq_t)32 * cache[i-3]
                         - (seq_t)16 * cache[i-4];
            }
        }
        highest = n;
    }

    return cache[n];
}
