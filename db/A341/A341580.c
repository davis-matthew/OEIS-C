/*
 * A341580  Number of steps needed to reach position "YZ^(n-1)" in the Towers
 *          of Hanoi exchanging disks puzzle with 3 pegs and n disks.
 * https://oeis.org/A341580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=12
 *            a(n)=2*a(n-1)-a(n-3)+2*a(n-4)-2*a(n-5) (n>=5)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: memoised values of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of values already computed */

    /* number of entries needed (0 … n inclusive) */
    size_t need = (size_t)n + 1;

    /* make sure the cache is large enough */
    if (need > cache_cap) {
        cache_cap = need;
        /* cast realloc result for C++ compatibility */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 as sentinel */

        /* initialise (or re‑initialise) the base values 0 … 4 */
        if (cache_cap > 0) cache[0] = 0;
        if (cache_cap > 1) cache[1] = 1;
        if (cache_cap > 2) cache[2] = 3;
        if (cache_cap > 3) cache[3] = 6;
        if (cache_cap > 4) cache[4] = 12;

        /* set the known length after initialisation */
        if (cache_len < 5) {
            cache_len = cache_cap < 5 ? cache_cap : 5;
        }
    }

    /* compute missing terms up to n */
    if (cache_len < need) {
        for (size_t i = cache_len; i < need; ++i) {
            if (i < 5) {
                /* base values already set */
                continue;
            }
            /* recurrence for n >= 5 */
            seq_t term = cache[i - 1];
            term = 2 * term;                     /* 2*a(n-1) */
            term -= cache[i - 3];                /* - a(n-3) */
            term += 2 * cache[i - 4];            /* + 2*a(n-4) */
            term -= 2 * cache[i - 5];            /* - 2*a(n-5) */
            cache[i] = term;
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}
