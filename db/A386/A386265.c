/*
 * A386265  G.f. A(x) satisfies A(x) = 1/(1+x) + x^2 * (d/dx A(x)^2).
 * https://oeis.org/A386265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n + (n-1) * Σ_{k=0..n-1} a(k) * a(n-1-k) with a(0) =
 *            1. This uses memoisation and an O(n²) convolution implementation
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* Ensure the cache is large enough. */
    if ((idx_t)(sz) <= n) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t newcap = cap ? cap * 2 : 1;
            while (newcap < needed) newcap *= 2;
            seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!newp) return 0;        /* memory allocation failure – unlikely in tests */
            cache = newp;
            cap   = newcap;
        }
        /* Compute any missing terms up to n. */
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i-1; ++k) {
                sum += cache[k] * cache[i-1-k];
            }
            seq_t sign = (i & 1) ? (seq_t)(-1) : (seq_t)1;
            cache[i] = sign + (seq_t)(i-1) * sum;
        }
        sz = (size_t)n + 1;
    }
    return cache[n];
}
