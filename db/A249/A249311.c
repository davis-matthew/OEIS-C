/*
 * A249311  Expansion of x*(1+9*x-8*x^3)/(1-10*x^2+8*x^4).
 * https://oeis.org/A249311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - 8*a(n-4), a(1)=1, a(2)=9, a(3)=10, a(4)=82
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1, return 0 */

    static seq_t *cache = NULL;      /* cached values, 1‑based */
    static idx_t cache_cap = 0;      /* allocated capacity (size+1) */

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t old_cap = cache_cap;
        seq_t *new_cache = seq_realloc(cache, old_cap, n + 1);
        if (!new_cache) abort();    /* out of memory */
        cache = new_cache;
        cache_cap = n + 1;

        /* compute values up to n (overwrite earlier entries – they are identical) */
        for (idx_t i = 1; i <= n; ++i) {
            if (i == 1)
                cache[1] = (seq_t)1;
            else if (i == 2)
                cache[2] = (seq_t)9;
            else if (i == 3)
                cache[3] = (seq_t)10;
            else if (i == 4)
                cache[4] = (seq_t)82;
            else
                cache[i] = (seq_t)10 * cache[i - 2] - (seq_t)8 * cache[i - 4];
        }
    }

    return cache[n];
}
