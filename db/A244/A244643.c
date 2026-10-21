/*
 * A244643  Number of cells with state 1 at n-th stage in some 2D reversible
 *          second order cellular automata (see comments for precise
 *          definition).
 * https://oeis.org/A244643
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* OEIS A244643: a(-1)=0; a(0)=1; a(2k+1)=4*a(k); a(2k+2)=a(k+1)+a(k). */
seq_t a(idx_t n)
{
    if (n < 0)
        return 0;

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t last = -1;          /* largest index already computed */

    /* expand the cache so that index n is valid */
    if (cap <= n) {
        idx_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;

        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
        if (cache) {
            for (idx_t i = 0; i <= last; ++i)
                new_cache[i] = cache[i];
            delete[] cache;
        }
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache) {
            memcpy(new_cache,
                   cache,
                   (last + 1) * sizeof(seq_t));
            free(cache);
        }
#endif
        cache = new_cache;
        cap = new_cap;
    }

    /* compute any new values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i & 1) {                   /* odd: a(2k+1)=4*a(k) */
            cache[i] = 4 * cache[(i - 1) >> 1];
        } else {                              /* even: a(2k+2)=a(k+1)+a(k) */
            idx_t k = (i - 2) >> 1;
            cache[i] = cache[k] + cache[k + 1];
        }
    }
    last = n;
    return cache[n];
}
