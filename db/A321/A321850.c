/*
 * A321850  E.g.f.: exp(x/(1-7*x)).
 * https://oeis.org/A321850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321850: a(n) = (14*n-13)*a(n-1) - 49*(n-2)*(n-1)*a(n-2),
   a(0) = 1, a(1) = 1 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static idx_t cache_len = 0;   /* number of elements currently stored */

/* Ensure that cache has room for indices 0..n.
   Add newly computed values using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)          /* already computed */
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_len];
    /* copy existing entries if any */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
#else
    new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failed */
#endif

    cache = new_cache;
    /* Compute missing terms from old cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else {
            seq_t t1 = ((seq_t)14 * (seq_t)i - (seq_t)13) * cache[i - 1];
            seq_t t2 = ((seq_t)49 * (seq_t)(i - 2) * (seq_t)(i - 1))
                       * cache[i - 2];
            cache[i] = t1 - t2;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
