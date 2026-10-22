/*
 * A376125  a(n) = 1 + Sum_{k=0..n-1} (2*k+1) * a(k) * a(n-k-1).
 * https://oeis.org/A376125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376125
   a(0) = 1
   a(n) = 1 + n * Σ_{k=0}^{n-1} a(k) * a(n-1-k)   for n ≥ 1
   (same as the equivalent formula with (2k+1) weights.) */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *a_cache = NULL;
static size_t cache_cap = 0;          /* allocated number of elements */
static idx_t  cache_len = 0;          /* number of computed elements */

/* Grow the cache to accommodate indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

#if defined(__cplusplus)
    /* C++: allocate a temporary array, copy, then replace */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = a_cache[i];
    delete[] a_cache;
    a_cache = tmp;
#else
    /* C: realloc works only for POD types */
    seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
#endif
    cache_cap = new_cap;
}

/* Compute a(n) with memoization and iterative convolution. */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                a_cache[0] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t k = 0; k < i; ++k)
                    sum += a_cache[k] * a_cache[i - 1 - k];
                a_cache[i] = (seq_t)i * sum + 1;
            }
        }
        cache_len = n + 1;
    }
    return a_cache[n];
}
