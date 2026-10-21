/*
 * A293050  Expansion of e.g.f. exp(x^4/(1 - x)).
 * https://oeis.org/A293050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)*a(n-1) - (n-2)*(n-1)*a(n-2) * +
 *            4*(n-3)*(n-2)*(n-1)*a(n-4) * - 3*(n-4)*(n-3)*(n-2)*(n-1)*a(n-5);
 *            * a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=24
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;
static size_t cache_alloc = 0;   /* allocated size */
static size_t cache_len  = 0;    /* number of computed entries */

/* Ensure that the cache has been allocated to hold indices up to n. */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)cache_alloc > n) return;
    size_t new_alloc = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
    if (tmp == NULL) exit(1);  /* allocation failed */
    cache = tmp;
#ifdef __cplusplus
    for (size_t i = cache_alloc; i < new_alloc; ++i)
        new(&cache[i]) seq_t();
#else
    for (size_t i = cache_alloc; i < new_alloc; ++i)
        cache[i] = 0;
#endif
    cache_alloc = new_alloc;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    if ((idx_t)cache_len > n) return cache[(size_t)n];

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1 || i == 2 || i == 3) {
            cache[i] = (seq_t)0;
        } else if (i == 4) {
            cache[i] = (seq_t)24;
        } else {
            idx_t idx = (idx_t)i;
            /* term1 = 2*(n-1)*a(n-1) */
            seq_t t1 = (seq_t)2 * (seq_t)(idx - 1) * cache[i - 1];
            /* term2 = (n-2)*(n-1)*a(n-2) */
            seq_t t2 = (seq_t)(idx - 2) * (seq_t)(idx - 1) * cache[i - 2];
            /* term3 = 4*(n-3)*(n-2)*(n-1)*a(n-4) */
            seq_t t3 = (seq_t)4 * (seq_t)(idx - 3) *
                       (seq_t)(idx - 2) * (seq_t)(idx - 1) * cache[i - 4];
            /* term4 = 3*(n-4)*(n-3)*(n-2)*(n-1)*a(n-5) */
            seq_t t4 = (seq_t)3 * (seq_t)(idx - 4) *
                       (seq_t)(idx - 3) * (seq_t)(idx - 2) *
                       (seq_t)(idx - 1) * cache[i - 5];
            /* a(n) = (t1 + t3) - (t2 + t4) */
            seq_t pos = t1 + t3;
            seq_t neg = t2 + t4;
            cache[i] = pos - neg;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
