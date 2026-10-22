/*
 * A370561  Periodic sequence S_{31} of length 28: used to compute the number
 *          that the day 31 of a month occurs on a day of the week for the
 *          Gregorian cycle of 400 years.
 * https://oeis.org/A370561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-4) + a(n-5) - a(n-8) + a(n-9) - a(n-12) +
 *            a(n-13) - a(n-16) + a(n-17) - a(n-20) + a(n-21) - a(n-24) +
 *            a(n-25) Initial values (0‑based index): a(0)=1, a(1)=0, a(2)=1,
 *            a(3)=1, a(4)=1, a(5)=1, a(6)=1, a(7)=0, a(8)=1, a(9)=1, a(10)=2,
 *            a(11)=1, a(12)=0, a(13)=1, a(14)=1, a(15)=1, a...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* A memoised implementation of the recurrence.  It works for
   arbitrary‑precision seq_t as well as for built‑in unsigned types. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* convert idx_t to size_t for array indices */
    size_t idx = (size_t)n;

    /* if the value is already cached, return it immediately */
    if (cache && idx < cache_size)
        return cache[idx];

    /* calculate how many entries we need eventually */
    size_t need = idx + 1;

    /* grow the cache if necessary */
    if (!cache) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache)
            return 0;                 /* allocation failure; fall back to 0 */
        cache_size = 0;
    } else if (need > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* allocation failure; fall back to 0 */
        cache = tmp;
    }

    /* known initial values (indices 0..24) */
    static const seq_t seed[25] = {
        1, 0, 1, 1, 1, 1, 1, 0,
        1, 1, 2, 1, 0, 1, 1, 1,
        2, 1, 0, 1, 1, 2, 1, 1, 1
    };

    /* compute any values that are still missing */
    for (size_t i = cache_size; i < need; ++i) {
        if (i <= 24) {
            cache[i] = seed[i];
        } else {
            /* recurrence using seq_t arithmetic only */
            cache[i] =  cache[i-1]
                       - cache[i-4] + cache[i-5]
                       - cache[i-8] + cache[i-9]
                       - cache[i-12] + cache[i-13]
                       - cache[i-16] + cache[i-17]
                       - cache[i-20] + cache[i-21]
                       - cache[i-24] + cache[i-25];
        }
    }

    /* update the known size of the cache */
    cache_size = need;

    return cache[idx];
}
