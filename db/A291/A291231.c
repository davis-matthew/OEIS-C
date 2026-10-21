/*
 * A291231  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)(1 - 2 S)(1 - 3
 *          S)(1 - 4 S).
 * https://oeis.org/A291231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A291231: p-INVERT of (0,1,0,1,0,1,...)
   Recurrence (n ≥ 7):
        a(n) = 10*a(n-1) - 31*a(n-2) + 20*a(n-3) + 40*a(n-4)
               - 20*a(n-5) - 31*a(n-6) - 10*a(n-7) - a(n-8)

   Initial values:
        a(0)=10, a(1)=65, a(2)=360, a(3)=1831, a(4)=8830,
        a(5)=41104, a(6)=186710, a(7)=833401
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* memory for computed terms          */
static size_t cache_cap = 0;         /* allocated capacity                  */
static size_t cached_to = 7;         /* highest index computed so far       */

seq_t a(idx_t n)
{
    /* --- initialisation of first eight terms --- */
    if (cache_cap == 0) {
        cache_cap   = 8;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – not expected */

        cache[0] = 10;
        cache[1] = 65;
        cache[2] = 360;
        cache[3] = 1831;
        cache[4] = 8830;
        cache[5] = 41104;
        cache[6] = 186710;
        cache[7] = 833401;
    }

    /* --- compute missing terms up to n --- */
    if ((size_t)n > cached_to) {
        /* enlarge cache if necessary */
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)n >= new_cap) new_cap <<= 1;       /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                               /* allocation fail */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute from cached_to+1 up to n (inclusive) */
        for (size_t i = cached_to + 1; i <= (size_t)n; ++i) {
            seq_t term = 10 * cache[i - 1]
                       - 31 * cache[i - 2]
                       + 20 * cache[i - 3]
                       + 40 * cache[i - 4]
                       - 20 * cache[i - 5]
                       - 31 * cache[i - 6]
                       - 10 * cache[i - 7]
                       -  cache[i - 8];
            cache[i] = term;
        }
        cached_to = (size_t)n;
    }

    return cache[(size_t)n];
}
