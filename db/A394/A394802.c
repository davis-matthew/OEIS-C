/*
 * A394802  G.f. A(x) satisfies A(x) = 1 - x^4 * d/dx log(1 - x*A(x)).
 * https://oeis.org/A394802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0)=1, a(1)=a(2)=a(3)=0; * for n≥1: a(n) = (n-3)*a(n-4) +
 *            Σ_{k=1}^{n-1} a(k)*a(n-1-k)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 31, u64 n <= 52, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
 
/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size */
    static idx_t  len   = 0;          /* number of values already computed (next free index) */
 
    /* lazily allocate the cache */
    if (len == 0) {
        cap = 8;                     /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        len = 1;
    }
 
    /* extend the cache up to index n (inclusive) if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *old = cache;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) {
            /* allocation failed, restore old pointer */
            cache = old;
            return (seq_t)0;
        }
        cap = new_cap;
    }
 
    /* compute missing entries using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        idx_t m = i - 1;          /* index for the convolution part */
        seq_t sum = 0;
        /* Σ_{k=0}^{m} a[k] * a[m-k]  (the generating‑function form) */
        for (idx_t k = 0; k <= m; ++k)
            sum += cache[k] * cache[m - k];
 
        /* a(i) = sum - a[i-1] + a[i-4] + (i>=5)*(i-4)*a[i-4]  (derived) */
        seq_t val = sum - cache[i - 1];
        if (i >= 4)
            val += cache[i - 4];
        if (i >= 5)
            val += (seq_t)(i - 4) * cache[i - 4];
 
        cache[i] = val;
    }
    len = n + 1;  /* now we have computed up to index n */
    return cache[n];
}
