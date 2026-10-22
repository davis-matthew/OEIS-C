/*
 * A377958  Expansion of e.g.f. exp(x - x^2)/(1 - x).
 * https://oeis.org/A377958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - 3*(n-1)*a(n-2) + 2*(n-1)*(n-2)*a(n-3) a(0)
 *            = 1, a(1) = 2, a(2) = 3 (cache 0..n computed once, reused on
 *            subsequent calls)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array holding computed values */
static idx_t  cache_len  = 0;      /* number of valid entries in cache   */
static idx_t  cache_cap  = 0;      /* allocated capacity of cache       */

seq_t a(idx_t n)
{
    /* Ensure cache has space for n+1 elements */
    if ((n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Fill base cases if not already present */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)2;
        cache_len = 2;
    }
    if (cache_len == 2 && n >= 2) {
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    /* Compute remaining values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3) continue;              /* base cases already set */
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];

        seq_t term1 = (seq_t)(i + 1) * a1;
        seq_t term2 = (seq_t)3 * (seq_t)(i - 1) * a2;
        seq_t term3 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(i - 2) * a3;

        cache[i] = term1 - term2 + term3;
    }

    cache_len = n + 1;   /* all entries up to n are now valid */
    return cache[n];
}
