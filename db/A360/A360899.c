/*
 * A360899  G.f. satisfies A(x) = 1 + x/(1 + x^4) * A(x/(1 + x^4)).
 * https://oeis.org/A360899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/4)} (-1)^k * binomial(n-1-3*k,k) *
 *            a(n-1-4*k), a(0) = 1
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    /* Initialise or grow cache as needed */
    if (!initialized) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;
        initialized = 1;
        last_computed = 0;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failed */
        memcpy(new_cache, cache, (last_computed + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 4;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial((i - 1) - 3 * k, k);
            seq_t term = bin * cache[i - 1 - 4 * k];
            if (k & 1) term = -term;
            sum = sum + term;
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
