/*
 * A365419  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+2) / (5*k+2)! ).
 * https://oeis.org/A365419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/5)} binomial(n-1,5*k+1) *
 *            a(n-5*k-2); initial values: a(0)=1, a(1)=0
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   last_done = 0;    /* largest n for which cache[i] is valid */

    /* --- Initialisation (n=0 & n=1) --------------------------------- */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        last_done = 1;
    }

    /* If the requested value is already cached, return it. */
    if (n <= last_done) {
        return cache[n];
    }

    /* Grow cache to accommodate 'n' */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from last_done+1 up to n (inclusive) */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; ; ++k) {
            idx_t m = 5 * k + 1;              /* 5*k+1 */
            if (m > i - 1) break;             /* out of range */
            idx_t sub = i - m - 1;            /* i-5*k-2 */
            seq_t term = binomial(i - 1, m);
            term = term * cache[sub];
            sum += term;
        }
        cache[i] = sum;
    }
    last_done = n;
    return cache[n];
}
