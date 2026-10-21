/*
 * A289428  Sum of factorial-Catalan numbers over a certain family of
 *          decorations.
 * https://oeis.org/A289428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * a(n-1) + 2 * sum_{k=1..n} a(k-1) * a(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of computed entries (len = max index + 1) */

seq_t a(idx_t n)
{
    /* grow cache if needed (doubling strategy) */
    if ((size_t)n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 1;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;           /* a(0) = 1 */
            } else {
                seq_t sum = (seq_t)0;
                /* sum_{k=1..i} a(k-1)*a(i-k) == sum_{k=0..i-1} a(k)*a(i-1-k) */
                for (idx_t k = 0; k < i; ++k) {
                    sum += cache[k] * cache[i - 1 - k];
                }
                cache[i] =
                    (seq_t)i * cache[i - 1] + (seq_t)2 * sum;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
