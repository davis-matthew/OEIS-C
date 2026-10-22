/*
 * A336183  a(n) = n^2 + (1/n) * Sum_{k=1..n-1} binomial(n,k) * k * a(k) *
 *          (n-k)^2.
 * https://oeis.org/A336183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + (1/n) * Σ_{k=1}^{n-1} binomial(n,k) * k * a(k) *
 *            (n-k)^2 Initial values: a(1) = 1 This implementation memoises
 *            computed values so calls can be in any order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;      /* memoised a(n) values */
static idx_t  cache_cap = 0;     /* capacity of cache (indices 0 .. cache_cap-1) */

/* Ensure cache can hold indices up to at least n.  Allocates new space if needed. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;   /* already enough space */

    idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= n)
        new_cap <<= 1;

#ifdef __cplusplus
    /* C++: use new[] so that constructors are invoked.  Copy existing data. */
    seq_t *newp = new seq_t[new_cap]();          /* value-initialised */
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
#else
    /* C: use realloc (no constructors, but seq_t is a built-in integer type). */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1.  Memoises results. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    static idx_t max_n = 0;   /* largest n for which a(k) has been computed */

    if (n <= max_n)          /* value already cached */
        return cache[n];

    /* Expand cache storage if necessary */
    ensure_cache(n);

    /* Compute successive values from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;   /* a(1) = 1 */
            continue;
        }

        seq_t sum = 0;
        seq_t binom = 1;   /* will hold C(i,k) during the k-loop */

        for (idx_t k = 1; k < i; ++k) {
            if (k == 1)
                binom = (seq_t)i;  /* C(i,1) = i */
            else
                binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;  /* C(i,k) */

            seq_t term = binom * (seq_t)k * cache[k];
            seq_t diff = (seq_t)(i - k);
            term *= diff * diff;       /* multiply by (i-k)^2 */
            sum += term;
        }

        cache[i] = (seq_t)i * (seq_t)i + sum / (seq_t)i;
    }

    max_n = n;
    return cache[n];
}
