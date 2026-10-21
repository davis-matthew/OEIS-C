/*
 * A294324  a(n) = Product_{k=0..n} (5*k + 2)!.
 * https://oeis.org/A294324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (5*k + 2)! (product of consecutive factorials)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;          /* array of products a(0) ... a(current) */
    static size_t cache_len = 0;         /* number of entries currently stored */
    static seq_t current_fact = 0;       /* (5*current_k + 2)! for the last k processed */
    static idx_t  current_k   = 0;       /* last k whose factorial was stored */

    /* extend the cache to cover index n, if necessary */
    if ((size_t)n >= cache_len)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* memory exhaustion – return zero */
        cache = tmp;

        /* compute missing terms */
        for (idx_t k = (idx_t)cache_len; k <= n; ++k)
        {
            if (k == 0)
            {
                /* base case: 2! = 2 */
                current_fact = factorial(2);
                current_k    = 0;
            }
            else
            {
                /* incrementally build (5*k + 2)! from the previous factorial */
                idx_t prev_m = 5 * current_k + 2;
                idx_t curr_m = 5 * k + 2;
                for (idx_t v = prev_m + 1; v <= curr_m; ++v)
                    current_fact *= (seq_t)v;
                current_k = k;
            }
            cache[k] = current_fact;   /* store a(k) = a(k-1) * (5*k + 2)!   */
            if (k > 0)
                cache[k] *= cache[k - 1];
        }
        cache_len = new_cap;
    }
    return cache[n];
}
