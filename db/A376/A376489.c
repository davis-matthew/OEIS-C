/*
 * A376489  G.f. satisfies A(x) = 1 / (1 - x^2*A(x)^2 / (1 - x)).
 * https://oeis.org/A376489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*k,k) *
 *            binomial(n-k-1,k-1) / (2*k+1). (binomial(n,k) is 0 for k<0 or
 *            k>n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes the utility that supplies binomial(idx_t, idx_t) */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* cached values */
static idx_t cache_size = 0;     /* allocated size of cache */
static idx_t cached_n   = 0;     /* maximum n that is cached (0..cached_n) */

seq_t a(idx_t n)
{
    /* initialise cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* allocate one element */
        if (!cache) abort();
        cache_size = 1;
        cache[0] = (seq_t)1;                     /* a(0) = 1 */
        cached_n   = 0;
    }

    /* grow cache if we need to reach index n */
    if ((idx_t)n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute new values up to n */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t maxk = i / 2;                     /* floor(i/2) */
            for (idx_t k = 1; k <= maxk; ++k) {
                seq_t term = binomial((idx_t)(3*k), k);
                term *= binomial(i - k - 1, k - 1);
                term /= (2 * k + 1);                /* 2*k+1 fits in idx_t */
                sum += term;
            }
            /* For i >= 1 the k=0 term contributes 0; a(0)=1 already set */
            cache[i] = sum;
        }
        cached_n = n;
    }

    return cache[n];
}
