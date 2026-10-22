/*
 * A390661  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * A(x^4))).
 * https://oeis.org/A390661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 + 2 * Sum_{k=0..floor((n-1)/4)} a(k) * a(n-1-4*k);
 * Offset 0: a(0) = 1
 *
 * This implementation memoises the sequence in a static array, computing
 * values lazily in O(n^2/4) time, which is fast enough for n up to ~10000.
 * The array is grown on demand using malloc/realloc, and the function
 * works for any build that supplies the types seq_t and idx_t.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Grow cache to at least n+1 elements */
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1 > cache_cap * 2) ? (n + 1) : (cache_cap * 2);
        if (new_cap == 0) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else {
            idx_t maxk = (i - 1) / 4;
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = i - 1 - 4 * k;
                sum += cache[k] * cache[j];
            }
            cache[i] = (seq_t)1 + (seq_t)2 * sum;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
