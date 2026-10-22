/*
 * A369842  Number of compositions of 5*n-1 into parts 2 and 5.
 * https://oeis.org/A369842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369842
   Formula (recurrence, n > 5): a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3)
                                 - 5*a(n-4) + a(n-5)
   Initial values:
   a(1)=1, a(2)=3, a(3)=7, a(4)=18, a(5)=52
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 – ignored */

    /* Cache of computed values, 0‑based: cache[0] == a(1) */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;          /* allocated capacity of cache */
    static idx_t  max_n  = 0;          /* largest n computed so far */

    /* Ensure the cache can hold up to n entries */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – propagate zero */
            return (seq_t)0;
        cache = tmp;
        /* zero‑init newly allocated slots */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Initialise the first five terms once */
    if (max_n < 5) {
        if (!cache) {
            cache = (seq_t *)calloc(5, sizeof(seq_t));
            if (!cache)
                return (seq_t)0;
            cap = 5;
        }
        /* a(1) ... a(5) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)52;
        max_n = 5;
    }

    if (n <= max_n)
        return cache[n - 1];

    /* Compute missing terms using the recurrence */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t val = (seq_t)5 * cache[k - 2];
        val -= (seq_t)9 * cache[k - 3];
        val += (seq_t)10 * cache[k - 4];
        val -= (seq_t)5 * cache[k - 5];
        val += cache[k - 6];
        cache[k - 1] = val;
    }
    max_n = n;
    return cache[n - 1];
}
