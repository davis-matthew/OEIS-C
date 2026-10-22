/*
 * A369848  Number of compositions of 5*n-4 into parts 3 and 5.
 * https://oeis.org/A369848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of 5*n-4 into parts 3 and 5 *
 *            Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) + 11*a(n-3) - 5*a(n-4) +
 *            a(n-5), * with a(1)=0, a(2)=1, a(3)=3, a(4)=6, a(5)=11. For n <
 *            1 the function * returns 0
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 1‑based cache: cache[1] holds a(1), etc.  cache[0] unused. */
static seq_t *cache = NULL;
static idx_t max_computed = 0;   /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n<1 */

    /* First time: allocate cache for indices 1..5 and set base values. */
    if (cache == NULL) {
        /* Allocate space for at least 6 entries (index 0 unused). */
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return (seq_t)0; /* allocation failure */

        cache[1] = (seq_t)0;    /* a(1) */
        cache[2] = (seq_t)1;    /* a(2) */
        cache[3] = (seq_t)3;    /* a(3) */
        cache[4] = (seq_t)6;    /* a(4) */
        cache[5] = (seq_t)11;   /* a(5) */
        max_computed = 5;
    }

    /* If the requested n is not yet cached, grow the cache and compute. */
    if (n > max_computed) {
        /* Grow cache to hold indices up to n (1‑based). */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* When i <= 5 we already know values, but the loop
             * starts at i = max_computed + 1 so that is safe. */
            seq_t term = (seq_t)5 * cache[i - 1];
            term -= (seq_t)10 * cache[i - 2];
            term += (seq_t)11 * cache[i - 3];
            term -= (seq_t)5 * cache[i - 4];
            term += cache[i - 5];
            cache[i] = term;
        }
        max_computed = n;
    }

    return cache[n];
}
