/*
 * A318267  a(n) is the number of configurations of n indistinguishable pairs
 *          placed on the vertices of the ladder graph P_2 X P_n such that all
 *          but two such pairs are joined by an edge.
 * https://oeis.org/A318267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 30, u64 n <= 72, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A318267:
 *   a(0)=0, a(1)=0, a(2)=1, a(3)=8, a(4)=39, a(5)=138, a(6)=414, a(7)=1104
 *   For n >= 8:
 *   a(n) = 5*a(n-1) - 7*a(n-2) - 2*a(n-3) + 10*a(n-4)
 *          - 2*a(n-5) - 5*a(n-6) + a(n-7) + a(n-8)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 .. 7 */
    static const seq_t init[8] = {0, 0, 1, 8, 39, 138, 414, 1104};

    /* cache for computed values */
    static seq_t *cache = NULL;   /* cache[0..cache_len-1] are valid */
    static idx_t cache_len = 0;   /* number of elements stored */

    /* trivial case: n < 8 */
    if (n < 8)
        return init[n];

    /* ensure cache is large enough and initialized */
    if (!cache || cache_len <= n) {
        idx_t old_len = cache_len;          /* store previous size */
        cache_len = n + 1;                  /* new required size          */

        seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!new_cache)                     /* out‑of‑memory: abort          */
            return (seq_t)-1;               /* caller will notice negative */
        cache = new_cache;

        /* initialise the first 8 entries if they are not yet set */
        if (old_len < 8) {
            for (idx_t i = old_len; i < 8; ++i)
                cache[i] = init[i];
            old_len = 8;
        }

        /* compute new entries using the recurrence */
        for (idx_t k = old_len; k <= n; ++k) {
            /* positive part   : 5*a(k-1) + 10*a(k-4) + a(k-7) + a(k-8) */
            seq_t pos = cache[k - 1] * (seq_t)5
                      + cache[k - 4] * (seq_t)10
                      + cache[k - 7]
                      + cache[k - 8];

            /* negative part: 7*a(k-2) + 2*a(k-3) + 2*a(k-5) + 5*a(k-6) */
            seq_t neg = cache[k - 2] * (seq_t)7
                      + cache[k - 3] * (seq_t)2
                      + cache[k - 5] * (seq_t)2
                      + cache[k - 6] * (seq_t)5;

            cache[k] = pos - neg;   /* pos >= neg for all k >= 8 */
        }
    }

    return cache[n];
}
