/*
 * A245834  E.g.f.: exp( x*(1 + exp(3*x)) ).
 * https://oeis.org/A245834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (3*k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 198 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                    /* size_t, NULL */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;      /* cache of computed values */
static size_t cache_cap = 0;     /* current capacity of cache */

seq_t a(idx_t n)
{
    /* Expand cache to store up to index n */
    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap  = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return 0;                 /* out of memory: 0 is a sentinel */
        cache = tmp;
        cache_cap = new_cap;

        for (idx_t i = old_cap; i <= n; ++i) {
            seq_t total = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t bin   = binomial(i, k);
                seq_t base  = (seq_t)(3ULL * k + 1);
                seq_t pow   = ipow(base, (seq_t)(i - k));
                total += bin * pow;
            }
            cache[i] = total;
        }
    }
    return cache[n];
}
