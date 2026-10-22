/*
 * A351973  a(n) = (-1)^n + Sum_{k=0..floor((n-1)/2)} a(k) * a(n-2*k-1).
 * https://oeis.org/A351973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..floor((n-1)/2)} a(k) * a(n-2*k-1)
 *            Initial values: a(0) = 1, a(1) = 0 The function memoizes results
 *            and works for calls in arbitrary order. All arithmetic is
 *            performed in the type seq_t and uses only integer operations
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static cache ----------
       cache_len: number of terms already computed (cache[0 .. cache_len-1])
       cache_cap: allocated capacity of the array
     */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise the cache on the first call */
    if (cache_len == 0) {
        cache_cap = 2;                         /* start with two slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1   */
        cache[1] = (seq_t)0;   /* a(1) = 0   */
        cache_len = 2;
    }

    /* grow the array if the requested index is beyond current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t kmax = (i <= 1) ? 0 : ((i - 1) / 2);
        for (idx_t k = 0; k <= kmax; ++k) {
            sum += cache[k] * cache[i - 2 * k - 1];
        }
        if ((i & 1ULL) == 0) {
            cache[i] = sum + (seq_t)1;        /* even i : add 1 */
        } else {
            cache[i] = sum - (seq_t)1;        /* odd  i : subtract 1 */
        }
    }

    cache_len = n + 1;
    return cache[n];
}
