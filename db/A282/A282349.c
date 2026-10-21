/*
 * A282349  Expansion of (Sum_{k>=0} x^(k*(2*k^2+1)/3))^7.
 * https://oeis.org/A282349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (Sum_{k>=0} x^(k*(2*k^2+1)/3))^7
 *            (number of ways to write n as an ordered sum of 7 octahedral
 *            numbers)
 * Range:     u8 n <= 25, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to memoize computed terms for all n >= 0 */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    /* return cached value if already computed */
    if (cache && n < cache_size)
        return cache[n];

    /* compute all values up to n */
    idx_t size = n + 1;

    /* build list of octahedral numbers <= n */
    size_t oct_cap = 32;
    idx_t *oct = (idx_t*)malloc(oct_cap * sizeof(idx_t));
    size_t oct_len = 0;
    for (idx_t k = 0;; ++k) {
        idx_t t = k * (((idx_t)2 * k * k) + 1) / 3;
        if (t > n) break;
        if (oct_len == oct_cap) {
            oct_cap *= 2;
            oct = (idx_t*)realloc(oct, oct_cap * sizeof(idx_t));
        }
        oct[oct_len++] = t;
    }

    /* 7‑fold convolution via dynamic programming */
    seq_t *cur  = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(size, sizeof(seq_t));
    cur[0] = (seq_t)1;   /* empty sum */

    for (int part = 1; part <= 7; ++part) {
        /* zero the next buffer */
        for (idx_t i = 0; i < size; ++i) next[i] = (seq_t)0;

        /* inner convolution */
        for (size_t idx = 0; idx < oct_len; ++idx) {
            idx_t t = oct[idx];
            for (idx_t i = t; i < size; ++i)
                next[i] += cur[i - t];
        }

        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* cur now holds the final results up to n */
    /* copy to cache */
    seq_t *new_cache = (seq_t*)malloc(size * sizeof(seq_t));
    for (idx_t i = 0; i < size; ++i)
        new_cache[i] = cur[i];

    /* free old cache if present */
    if (cache) free(cache);
    cache = new_cache;
    cache_size = size;

    /* release temporary arrays */
    free(cur);
    free(next);
    free(oct);

    return cache[n];
}
