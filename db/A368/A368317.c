/*
 * A368317  Expansion of e.g.f. exp(4*x) / (2 - exp(x)).
 * https://oeis.org/A368317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A368317
   Formula (recurrence): a(0) = 1
   a(n) = 4^n + Σ_{k=1..n} binomial(n,k) * a(n-k)   (n ≥ 1)
   The terms are indexed starting at 0. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Cache for previously computed terms. */
    static seq_t *cache   = NULL;   /* array holding values a(0) .. a(max) */
    static idx_t  cache_len   = 0;   /* number of elements actually initialized */
    static size_t cache_alloc = 0;   /* allocated size (power of two is ok) */

    /* Ensure we have enough storage for indices up to n. */
    if (cache_alloc <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – caller may handle */
        cache     = tmp;
        cache_alloc = new_cap;
    }

    /* Initialise a(0) if it has not been set yet. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_len = 1;
    }

    /* If the requested term is already computed, return it. */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* Compute missing terms up to index n. */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        seq_t sum = 0;                /* Σ_{k=1..cur} binomial(cur,k) * a(cur-k) */
        seq_t C   = 1;                /* binomial(cur,0) initially */

        /* Iterate over k = 1 .. cur (implement as k = 0 .. cur-1 with shift). */
        for (idx_t k = 0; k < cur; ++k) {
            /* Advance C to binomial(cur, k+1). */
            C = (C * (seq_t)(cur - k)) / (seq_t)(k + 1);
            /* Add contribution of a(cur-k-1) multiplied by the new binomial. */
            sum += C * cache[cur - 1 - k];
        }

        cache[cur] = pow_uint(4, cur) + sum;   /* a(cur) = 4^cur + sum */
    }

    /* Update the number of computed terms. */
    cache_len = n + 1;
    return cache[n];
}
