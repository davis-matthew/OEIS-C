/*
 * A279684  The maximum number of coins that can be processed in n weighings
 *          that all are real except for one LHR-coin starting in the heavy or
 *          real state.
 * https://oeis.org/A279684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) - 8*a(n-5) initial
 *            values: a(0)=1, a(1)=3, a(2)=5, a(3)=15, a(4)=37, a(5)=87
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* allocated capacity of the cache   */
    static size_t high = 0;          /* first index not yet computed + 1  */

    /* base values for n = 0..5 */
    static const seq_t base[6] = { (seq_t)1, (seq_t)3, (seq_t)5,
                                   (seq_t)15, (seq_t)37, (seq_t)87 };

    /* return early for small indices */
    if (n < 6)
        return base[n];

    /* initialise the cache on first call */
    if (cache == NULL) {
        /* initialise to hold the first six values */
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        high = 6;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= high) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap;
            while (newcap < need)
                newcap = (newcap == 0) ? 6 : newcap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = newcap;
        }
        /* compute missing terms using the recurrence */
        for (size_t i = high; i <= (size_t)n; ++i) {
            seq_t term = (seq_t)0;
            term += (seq_t)3 * cache[i - 1];   /* +3*a(i-1) */
            term -= cache[i - 2];              /* - a(i-2)  */
            term += cache[i - 3];              /* + a(i-3)  */
            term -= (seq_t)2 * cache[i - 4];   /* -2*a(i-4) */
            term -= (seq_t)8 * cache[i - 5];   /* -8*a(i-5) */
            cache[i] = term;
        }
        high = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
