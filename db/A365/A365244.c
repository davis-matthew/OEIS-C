/*
 * A365244  G.f. satisfies A(x) = 1 + x*A(x)/(1 - x^2*A(x)^3).
 * https://oeis.org/A365244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k-1,k) *
 *            binomial(n+k+1,n-2k)/(n+k+1) , a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation; works for any call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* ensure cache has room for up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();                      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 1) {
            val = 1;                     /* a(0)=a(1)=1 */
        } else {
            idx_t limit = i / 2;
            seq_t sum = 0;
            for (idx_t k = 0; k <= limit; ++k) {
                seq_t t1 = binomial(i - k - 1, k);
                seq_t t2 = binomial(i + k + 1, i - 2 * k);
                seq_t t = t1 * t2;
                t /= (seq_t)(i + k + 1);
                sum += t;
            }
            val = sum;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
