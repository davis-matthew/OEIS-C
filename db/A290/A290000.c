/*
 * A290000  a(n) = Product_{k=1..n-1} (3^k + 1).
 * https://oeis.org/A290000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n-1} (3^k + 1) Recurrence: a(0) = 1, a(1) = 1,
 *            a(n) = a(n-1) * (3^{n-1} + 1) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache array */
    static idx_t  cache_len = 0;   /* number of valid entries (indices 0 .. cache_len-1) */
    static seq_t pow3_cur = 0;     /* 3^{cache_len-1} – current power of three */

    /* first call – initialise the cache */
    if (cache == NULL) {
        cache_cap = (size_t)(n) + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
        pow3_cur = (seq_t)3;   /* 3^1 for the first multiplication */
    }
    /* enlarge cache if necessary */
    else if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n) + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = pow3_cur + (seq_t)1;   /* 3^{i-1} + 1 */
        cache[i] = cache[i - 1] * term;
        pow3_cur *= (seq_t)3;                /* advance to 3^i for next step */
        cache_len = i + 1;
    }

    return cache[n];
}
