/*
 * A254664  Numbers of words on alphabet {0,1,...,8} with no subwords ii,
 *          where i is from {0,1,...,5}.
 * https://oeis.org/A254664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(n)=8*a(n-1)+3*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n‑th term of OEIS sequence A254664.
   The function is memoised; any call order is supported. */
seq_t a(idx_t n)
{
    /* Static cache for computed values: indices 0 … cache_capacity-1 */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* number of entries currently stored */

    /* First call – allocate a minimal table (at least two entries)
       and compute up to n (or one if n<1). */
    if (cache == NULL) {
        idx_t new_cap = (n + 1 > (idx_t)2 ? n + 1 : (idx_t)2);
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)new_cap);
        if (!cache) return 0;                /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        for (idx_t i = 2; i < new_cap; ++i)
            cache[i] = (seq_t)8 * cache[i - 1] + (seq_t)3 * cache[i - 2];

        cache_cap = new_cap;
        return cache[n];
    }

    /* Cache exists – enlarge if necessary, computing the missing terms. */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      sizeof(seq_t) * (size_t)new_cap);
        if (!tmp) return 0;                 /* reallocation failed */

        /* Compute all new entries using the recurrence. */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)8 * tmp[i - 1] + (seq_t)3 * tmp[i - 2];

        cache     = tmp;
        cache_cap = new_cap;
    }

    return cache[n];
}
