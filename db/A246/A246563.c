/*
 * A246563  G.f.: Sum_{n>=0} x^n / (1-x^2)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          x^(2*k)]^2.
 * https://oeis.org/A246563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A246563
   Formula: a(n) = Sum_{k=0..⌊n/2⌋} Sum_{j=0..k} C(n-k-j, k)^2 * C(k, j)^2
   The implementation evaluates the double sum on demand, caching the
   results so that subsequent calls are O(1).  The cache grows geometrically,
   and values are computed only once.  The code works for any seq_t
   (fixed width or arbitrary precision) and any idx_t (uint64_t).  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* static cache of computed terms */
static seq_t *cache = NULL;   /* pointer to array of seq_t */
static idx_t cache_len = 0;   /* number of entries currently stored */

/* Grow the cache and compute new entries (if any) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Determine new capacity: at least n+1, doubled each time */
    size_t new_cap = cache_len ? cache_len * 2 : 16;
    while (new_cap <= (size_t)(n + 1))
        new_cap *= 2;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* Compute missing terms, starting from the first new index */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else if (i == 3) {
            cache[i] = (seq_t)6;
        } else if (i == 4) {
            cache[i] = (seq_t)15;
        } else {
            /* Double‑sum evaluation */
            seq_t sum = 0;
            for (idx_t k = 0; k <= i / 2; ++k) {
                /* For j = 0 */
                seq_t bin_k_j = 1;                       /* C(k,0)   */
                seq_t bin_nk_k = binomial(i - k, k);     /* C(i‑k,k) */
                sum += bin_k_j * bin_k_j * bin_nk_k * bin_nk_k;

                /* j runs up to min(k, i‑2k) */
                idx_t max_j = (i >= 2 * k) ? k : (i - 2 * k);
                for (idx_t j = 1; j <= max_j; ++j) {
                    /* Update C(k,j) from C(k,j‑1) */
                    bin_k_j = bin_k_j * (seq_t)(k - j + 1) / (seq_t)j;

                    /* Update C(i‑k‑j, k) from previous value */
                    idx_t prev_a = i - k - (j - 1);          /* a = i‑k‑(j‑1) */
                    seq_t numerator   = (seq_t)(prev_a - k);   /* a‑k   */
                    seq_t denominator = (seq_t)prev_a;        /* a     */
                    bin_nk_k = bin_nk_k * numerator / denominator;

                    sum += bin_k_j * bin_k_j * bin_nk_k * bin_nk_k;
                }
            }
            cache[i] = sum;
        }
    }
    cache_len = n + 1;
}

/* External entry point required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
