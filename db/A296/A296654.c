/*
 * A296654  a(n) = a(n-1)^(n-1) + a(n-2)^(n-1) with a(0)=0 and a(1)=1.
 * https://oeis.org/A296654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^(n-1) + a(n-2)^(n-1) Initial values: a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms.
       cache[i] holds a(i).  max_comp records the largest
       index that has been computed. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_comp = 0;

    /* Ensure the cache is big enough for the requested index. */
    if (cache == NULL)
    {
        /* Allocate for at least 2 terms (a(0) and a(1)). */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (cache == NULL) return 0; /* fallback on OOM */
        cache[0] = 0;
        cache[1] = 1;
        cap = 2;
        max_comp = 1;
    }

    if ((size_t)n >= cap)
    {
        /* Grow the cache to accommodate n+1 entries. */
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) return 0; /* OOM fallback */
        cache = new_cache;
        cap = new_cap;
    }

    if (n <= max_comp)
        return cache[n];

    /* Compute terms up to n inclusively. */
    for (idx_t i = max_comp + 1; i <= n; ++i)
    {
        /* Exponent is (i-1); cast to seq_t for big_pow. */
        seq_t pow_prev = big_pow(cache[i - 1], (seq_t)(i - 1));
        seq_t pow_prev2 = big_pow(cache[i - 2], (seq_t)(i - 1));
        cache[i] = pow_prev + pow_prev2;
    }

    max_comp = n;
    return cache[n];
}
