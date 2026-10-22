/*
 * A365909  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(5*k+2) / (5*k+2)! ).
 * https://oeis.org/A365909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊(n-2)/5⌋} binomial(n,5k+2) * a(n-5k-2) initial
 *            values: a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Initialise cache and base cases */
    if (!cache) {
        cache_cap = (n > 64 ? n + 1 : 64);
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;                  /* a(0) */
        cache[1] = 0;                  /* a(1) */
        cache[2] = 1;                  /* a(2) */
        cache_size = 3;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        idx_t max_k = 0;
        if (i >= 2)
            max_k = (i - 2) / 5;
        for (idx_t k = 0; k <= max_k; ++k) {
            idx_t j = 5 * k + 2;
            seq_t bin = binomial(i, j);
            seq_t term = cache[i - j];
            sum += bin * term;
        }
        cache[i] = sum;
    }
    cache_size = n + 1;

    return cache[n];
}
