/*
 * A320097  Number of no-leaf subgraphs of the 4 X n grid.
 * https://oeis.org/A320097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 7*a(n-2) - 201*a(n-3) + 49*a(n-4) + 20*a(n-5)
 *            - 5*a(n-6) Initial values: a(1)=1, a(2)=15, a(3)=463,
 *            a(4)=16372, a(5)=583199, a(6)=20788249
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache   */
    static idx_t  computed = 0;    /* highest n already computed */

    if (n == 0) {                 /* safety for n < 1 */
        return (seq_t)0;
    }

    /* enlarge cache array if necessary */
    size_t nsize = (size_t)n;
    if (nsize + 1 > cache_cap) {
        size_t newcap = nsize + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();        /* out‑of‑memory */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms up to n */
    if (computed < n) {
        for (size_t i = (size_t)(computed + 1); i <= nsize; ++i) {
            if (i <= 6) {          /* initialise the base cases */
                if (i == 1) cache[i] = (seq_t)1;
                else if (i == 2) cache[i] = (seq_t)15;
                else if (i == 3) cache[i] = (seq_t)463;
                else if (i == 4) cache[i] = (seq_t)16372;
                else if (i == 5) cache[i] = (seq_t)583199;
                else            cache[i] = (seq_t)20788249;
            } else {
                /* recurrence: a(i) = 36*a(i-1) - 7*a(i-2) - 201*a(i-3)
                                   + 49*a(i-4) + 20*a(i-5) - 5*a(i-6) */
                seq_t res = cache[i-1] * (seq_t)36
                         + cache[i-4] * (seq_t)49
                         + cache[i-5] * (seq_t)20;
                res -= cache[i-2] * (seq_t)7;
                res -= cache[i-3] * (seq_t)201;
                res -= cache[i-6] * (seq_t)5;
                cache[i] = res;
            }
        }
        computed = n;            /* update cache status */
    }

    return cache[nsize];
}
