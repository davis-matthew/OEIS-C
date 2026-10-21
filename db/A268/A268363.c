/*
 * A268363  Number of n X 2 arrays containing 2 copies of 0..n-1 with row sums
 *          equal.
 * https://oeis.org/A268363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^floor(n/2) * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdlib.h> /* for malloc and realloc */

seq_t a(idx_t n)
{
    /* Cache of factorials: fact_cache[i] = i!  (0! = 1) */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cache_size = 0;      /* highest n currently cached */
    static size_t fact_cache_cap = 0;      /* allocated capacity */

    /* Initialise cache on first call. */
    if (!fact_cache) {
        fact_cache_cap = 1;
        fact_cache = (seq_t*)malloc(fact_cache_cap * sizeof(seq_t));
        if (!fact_cache) return 0;          /* allocation failed: return sentinel */
        fact_cache[0] = 1;                  /* 0! = 1 */
        fact_cache_size = 0;
    }

    /* Grow the cache if needed. */
    if (n > fact_cache_size) {
        /* Ensure capacity up to n+1 elements. */
        if (fact_cache_cap <= (size_t)n) {
            size_t new_cap = fact_cache_cap * 2;
            if (new_cap <= (size_t)n) new_cap = n + 1;
            seq_t *tmp = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;              /* allocation failed */
            fact_cache = tmp;
            fact_cache_cap = new_cap;
        }

        /* Compute missing factorials. */
        for (idx_t i = fact_cache_size + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = n;
    }

    /* a(n) = 2^floor(n/2) * n! */
    seq_t pow_val = pow2(n / 2);
    return pow_val * fact_cache[n];
}
