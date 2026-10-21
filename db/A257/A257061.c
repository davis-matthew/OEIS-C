/*
 * A257061  Number of length n 1..(7+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=27, a(3)=141, a(4)=738
 *            a(n)=4*a(n-1)+5*a(n-2)+7*a(n-3)+4*a(n-4) for n≥5 (OEIS A257061)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array indexed from 1 */
    static idx_t  cached_upto = 0;     /* highest index computed so far */
    static size_t cached_cap  = 0;     /* allocated size (in elements) */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* make room for at least the first four terms, plus a dummy 0* */
        cached_cap = 5;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* seed the first four terms */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)27;
        cache[3] = (seq_t)141;
        cache[4] = (seq_t)738;

        cached_upto = 4;
    }

    /* grow the capacity if required for the requested n */
    if ((size_t)(n + 1) > cached_cap) {
        cached_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* reallocation failed */
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += cache[i-1] * (seq_t)4;
        val += cache[i-2] * (seq_t)5;
        val += cache[i-3] * (seq_t)7;
        val += cache[i-4] * (seq_t)4;
        cache[i] = val;
    }

    /* update cache info */
    if (n > cached_upto) cached_upto = n;

    return cache[n];
}
