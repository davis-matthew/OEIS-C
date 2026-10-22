/*
 * A385946  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+4,4) * binomial(n-1,k)
 *          * a(k) * a(n-1-k).
 * https://oeis.org/A385946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(k+4,4) * binomial(n-1,k) * a(k) *
 *            a(n-1-k) * Initial values: a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binom4_from_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom4_from_k.h"

/* Helper to compute binomial(n,k) multiplicatively */
static seq_t binom_multiplicative(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k; /* symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

/* Memoized computation of a(n) */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* dynamic array of computed values */
    static size_t cache_size = 0;        /* current number of slots (>= cache_size) */

    if (n == 0) return (seq_t)1;

    /* Ensure cache has space for [0..n] */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        /* Initialise new slots to zero */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Base value */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;

    /* Compute missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (cache[idx] == (seq_t)0) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t term = binom4_from_k(k);
                term *= binom_multiplicative(i - 1, k);
                term *= cache[(size_t)k];
                term *= cache[(size_t)(i - 1 - k)];
                sum += term;
            }
            cache[idx] = sum;
        }
    }

    return cache[(size_t)n];
}
