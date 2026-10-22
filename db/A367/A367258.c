/*
 * A367258  G.f. satisfies A(x) = 1 + x*A(x) * (1 + x*A(x)^2)^2.
 * https://oeis.org/A367258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n - k + 1, k) * binomial(2*k, n -
 *            k) / (2*n - k + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A367258 from OEIS */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t max_comp = 1;      /* largest n that has been computed */
static idx_t cache_cap = 2;     /* allocated size of cache */

/* External function as requested */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Ensure cache large enough */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_comp = 1;
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n if necessary */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            idx_t n1 = 2 * i - k + 1;
            idx_t n2 = 2 * k;
            idx_t k1 = k;
            idx_t k2 = i - k;

            seq_t bin1 = binomial(n1, k1);
            seq_t bin2 = binomial(n2, k2);
            seq_t denom = (seq_t)(n1);
            seq_t prod  = bin1 * bin2;
            seq_t term  = prod / denom;   /* division is exact */
            sum += term;
        }
        cache[i] = sum;
        max_comp = i;
    }

    return cache[n];
}
