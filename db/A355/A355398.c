/*
 * A355398  Expansion of e.g.f. exp(exp(3*x)/3 - exp(2*x)/2 + 1/6).
 * https://oeis.org/A355398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (3^(k-1)-2^(k-1)) * binomial(n-1,k-1) *
 *            a(n-k) initial value: a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;          /* memoisation cache */
static idx_t cache_len  = 0;         /* allocated cache length */
static idx_t cached_up_to = 0;       /* highest n computed (inclusive) */

/* Ensure the cache holds values up to n and computes any missing ones. */
static void ensure_cache(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cached_up_to = 0;
    }

    if (n <= cached_up_to)
        return;                /* already computed */

    /* grow storage if necessary */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = cache_len ? cache_len : 1;
        while (new_len < (size_t)(n + 1))
            new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) exit(1);
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms iteratively */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t pow3 = 1;   /* 3^(k-1) */
        seq_t pow2 = 1;   /* 2^(k-1) */

        for (idx_t k = 1; k <= i; ++k) {
            if (k > 1) {
                pow3 = mul_small(pow3, 3);
                pow2 = mul_small(pow2, 2);
            }
            seq_t diff = pow3 - pow2;
            seq_t bin  = binomial(i - 1, k - 1);
            seq_t term = bin * diff;
            term = term * cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }
    cached_up_to = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
