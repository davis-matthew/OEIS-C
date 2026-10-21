/*
 * A283819  a(n) = (1 + Sum_{j=1..K-2} a(n-j)*a(n-j-1))/a(n-K) with
 *          a(1),...,a(K)=1, where K=5.
 * https://oeis.org/A283819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + ∑_{j=1..3} a(n-j)*a(n-j-1)) / a(n-5) with
 *            a(1)=a(2)=a(3)=a(4)=a(5)=1 (K = 5)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms – 1‑based index */
    static seq_t *cache   = NULL;          /* array of seq_t */
    static size_t cache_cap = 0;           /* allocated size (≥ 6) */
    static idx_t  cache_used = 0;          /* highest index computed */

    /* Ensure we have space for indices 1 .. n (inclusive) */
    /* The cache must always have room for at least indices 1..5 (i.e. size ≥ 6) */
    size_t required_cap = (size_t)n + 1;
    if (required_cap < 6)
        required_cap = 6;        /* 6 = (5 + 1) */
    if (required_cap > cache_cap) {
        void *ptr = realloc(cache, required_cap * sizeof(seq_t));
        if (!ptr) abort();      /* out of memory – terminate */
        cache     = (seq_t *)ptr;
        cache_cap = required_cap;
    }

    /* Initialise the first five terms once (they will stay 1)**/
    if (cache_used < 5) {
        /* cache has at least 6 elements due to the allocation above */
        cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = (seq_t)1;
        cache_used = 5;
    }

    /* If the requested term is already known, return it */
    if (n <= cache_used)
        return cache[n];

    /* Compute terms from cache_used + 1 up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t sum = (seq_t)1;
        /* j = 1 : a(i-1)*a(i-2) */
        sum += cache[i-1] * cache[i-2];
        /* j = 2 : a(i-2)*a(i-3) */
        sum += cache[i-2] * cache[i-3];
        /* j = 3 : a(i-3)*a(i-4) */
        sum += cache[i-3] * cache[i-4];

        seq_t denom = cache[i-5];
        cache[i] = sum / denom;  /* integer division – denom never zero */
    }

    cache_used = n;
    return cache[n];
}
