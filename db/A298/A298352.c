/*
 * A298352  a(n) = a(n-1) + a(n-2) + a([(n-1)/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(floor((n-1)/2)), a(0) = 1, a(1) = 1,
 *            a(2) = 1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache  = NULL;   /* array of terms */
    static size_t cap    = 0;      /* allocated capacity */
    static idx_t  maxN   = 0;      /* highest index computed so far */

    /* base cases */
    if (n <= 2) {
        return (seq_t)1;
    }

    /* ensure cache large enough for index n */
    if ((size_t)(n + 1) > cap) {
        size_t newCap = (cap ? cap * 2 : 4);
        while (newCap <= (size_t)n + 1)
            newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) {
            abort();            /* allocation failure */
        }
        if (tmp != cache) {
            /* zero the newly allocated part */
            memset(tmp + cap, 0, (newCap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap   = newCap;
    }

    /* initialise base values once */
    if (maxN < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        maxN = 2;
    }

    /* compute missing terms up to n, using dynamic programming */
    for (idx_t i = maxN + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[(i - 1) / 2];
    }
    maxN = n;

    return cache[n];
}
