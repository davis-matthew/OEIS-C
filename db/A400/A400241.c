/*
 * A400241  G.f. A(x) satisfies A(x) = A(x^2/(1 - x)^3) / (1 - x), with A(0) =
 *          1.
 * https://oeis.org/A400241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400241
   Formula: a(n) = Sum_{k=0..floor(n/2)} binomial(n+k,3*k) * a(k)
   Initial value: a(0) = 1
   Recurrence uses an indexed cache so calls may be in any order. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current capacity of cache */
    static idx_t max_computed = 0;  /* largest index already computed */

    /* initialise cache on first call */
    if (!cache) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* OOM, return 0 as safe default */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        max_computed = 0;
    }

    /* if a(n) already computed, return it immediately */
    if (n <= max_computed)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;            /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failed, return 0 */
        cache = tmp;
        /* new slots will be filled in the loop below (no need to zero them) */
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t max_k = i >> 1;           /* floor(i/2) */
        for (idx_t k = 0; k <= max_k; ++k) {
            seq_t bin = binomial(i + k, 3 * k);
            sum += bin * cache[k];
        }
        cache[i] = sum;
    }
    max_computed = n;

    return cache[n];
}
