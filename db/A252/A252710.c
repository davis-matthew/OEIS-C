/*
 * A252710  Sum_{k=0..n} k^(n-k)*(n+k)^k.
 * https://oeis.org/A252710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0..n} k^(n-k) * (n+k)^k
 * Offset: 1  (a(1) = 2)
 */
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static seq_t *cache = NULL;   /* cache[1] holds a(1), etc. */
static size_t cache_cap = 0;  /* allocated array size (>= needed + 1) */

/* Compute a(n) for n >= 1, using memoization. */
seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure, return 0 */
        /* copy existing values */
        for (size_t i = 1; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* initialize new slots to zero (0 cast to seq_t) */
        for (size_t i = cache_cap; i <= n; ++i)
            new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute a(n) from scratch */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term1 = big_pow((seq_t)k, (seq_t)(n - k));
        seq_t term2 = big_pow((seq_t)(n + k), (seq_t)k);
        seq_t prod  = seq_mul(term1, term2);
        sum += prod;
    }

    cache[n] = sum;
    return sum;
}
