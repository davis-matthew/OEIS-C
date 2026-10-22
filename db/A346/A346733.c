/*
 * A346733  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 * A(x)^3.
 * https://oeis.org/A346733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=1; for n≥3: a(n)=∑_{i=0}^{n‑3} ∑_{j=0}^{n‑i‑3}
 *            a(i)·a(j)·a(n‑i‑j‑3)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static tables for memoization */
    static seq_t *cache   = NULL;  /* a[0..cache_len-1]   */
    static size_t cache_len = 0;   /* number of stored terms */

    static seq_t *conv    = NULL;  /* convolution sums */
    static size_t conv_len  = 0;   /* size of conv array */

    if (n < cache_len)          /* already computed */
        return cache[(size_t)n];

    /* enlarge a[] if necessary */
    size_t req_cache = (size_t)n + 1;
    size_t old_cache_len = cache_len;
    if (req_cache > cache_len) {
        cache = (seq_t *)realloc(cache, req_cache * sizeof(seq_t));
        cache_len = req_cache;
    }

    /* enlarge convolution array if necessary */
    size_t req_conv = 2 * (size_t)n + 1;
    if (req_conv > conv_len) {
        conv = (seq_t *)realloc(conv, req_conv * sizeof(seq_t));
        for (size_t i = conv_len; i < req_conv; ++i)
            conv[i] = 0;
        conv_len = req_conv;
    }

    /* compute missing terms */
    for (size_t idx = old_cache_len; idx <= (size_t)n; ++idx) {
        if (idx <= 2) {                 /* initial values */
            cache[idx] = 1;
        } else {
            seq_t sum = 0;
            for (size_t i = 0; i <= idx - 3; ++i)
                sum += conv[i] * cache[idx - 3 - i];
            cache[idx] = sum;
        }

        /* update convolution array with the new a[idx] */
        for (size_t t = idx; t <= 2 * idx; ++t) {
            seq_t prod = cache[idx] * cache[t - idx];
            conv[t] += prod;
            if (t - idx != idx)          /* avoid double‐counting when t = 2*idx */
                conv[t] += prod;
        }
    }

    return cache[(size_t)n];
}
