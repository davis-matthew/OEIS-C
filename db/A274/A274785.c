/*
 * A274785  Diagonal of the rational function 1/(1-(w*x*y*z + w*x*z + w*y +
 *          x*y + z)).
 * https://oeis.org/A274785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n+2k,2k)*C(n,2k)*C(2k,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the terms on demand and caches them for future calls. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t n, idx_t k) returning seq_t */

/* External types supplied by the build */
typedef uint64_t idx_t;   /* index type is always unsigned */

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n) {
    if (n < cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new elements to zero */
    for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + 2 * k, 2 * k);
        seq_t t2 = binomial(n, 2 * k);
        seq_t t3 = binomial(2 * k, k);
        seq_t term = t1 * t2;
        term = term * t3;
        term = term * t3;
        sum += term;
    }
    cache[n] = sum;
    return sum;
}
