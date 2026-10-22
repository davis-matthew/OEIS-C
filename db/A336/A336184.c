/*
 * A336184  a(n) = n^3 + (1/n) * Sum_{k=1..n-1} binomial(n,k) * k * a(k) *
 *          (n-k)^3.
 * https://oeis.org/A336184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 + (1/n) * Sum_{k=1..n-1} binomial(n,k) * k * a(k) *
 *            (n-k)^3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(1) = 1 */

#include <stdlib.h>   /* malloc, realloc, free, abort */

#ifdef __cplusplus
/* For C++ builds the compiler will provide the definition of seq_t and idx_t */
static seq_t *cache = nullptr;
static idx_t  cache_max = 0;      /* largest n that has been computed */
#else
static seq_t *cache = NULL;
static idx_t  cache_max = 0;
#endif

/* Expand the cache up to at least n (inclusive).  The routine takes care of
   allocation and copying of already known values, and it works for both
   C and C++ builds (new/delete are used in C++; malloc/realloc in C). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;                     /* nothing to do */

    /* number of slots needed: indices 0..n inclusive */
    size_t new_cap = (size_t)n + 1;

    /* Allocate new storage if larger than currently allocated */
    if (new_cap > (size_t)cache_max) {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        if (cache) {
            for (idx_t i = 1; i <= cache_max; ++i) new_cache[i] = cache[i];
            delete[] cache;                          /* free old storage */
        }
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();                    /* allocation failed */
        cache = new_cache;
#endif
    }

    /* Compute missing values from cache_max + 1 up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;                                 /* binomial(m,0) */

        for (idx_t k = 1; k < m; ++k) {
            /* Update binomial(m,k) from binomial(m,k-1). */
            bin = (bin * (seq_t)(m - k + 1)) / k;

            seq_t diff = (seq_t)(m - k);               /* (m-k) */
            seq_t diff_sq = diff * diff;
            seq_t diff_cu = diff_sq * diff;

            /* term = bin * k * a(k) * (m-k)^3 */
            seq_t term = bin * (seq_t)k;
            term = term * cache[k];
            term = term * diff_cu;

            sum += term;
        }

        seq_t m_cub = (seq_t)m * (seq_t)m * (seq_t)m;
        seq_t a_val = m_cub + sum / (seq_t)m;
        cache[m] = a_val;
    }

    cache_max = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but handle gracefully */
    ensure_cache(n);
    return cache[n];
}
