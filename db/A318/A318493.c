/*
 * A318493  Expansion of 1/(1 - Sum_{i>=1, j>=1} i*j*x^(i*j)).
 * https://oeis.org/A318493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A318493: a(0) = 1, a(n) = Σ_{k=1..n} (k * d(k)) * a(n–k),
 * where d(k) is the number of divisors of k.
 */

#include <stdlib.h>
#include <stdint.h>

/* Function to compute the sequence value a(n). */
seq_t a(idx_t n)
{
    /* Cached sequence values and their capacity. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static idx_t computed_max = 0;       /* largest n already computed */

    /* Cached divisor counts and their capacity. */
    static uint32_t *divcnt = NULL;
    static size_t divcnt_sz = 0;
    static idx_t div_max_computed = 0;   /* largest n for which divcnt is valid */

    /* Grow the cache and divisor arrays if needed. */
    size_t needed = (size_t)n + 1;       /* need room for index n */
    if (cache_sz < needed) {
        if (cache_sz == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        }
        cache_sz = needed;
    }
    if (divcnt_sz < needed) {
        if (divcnt_sz == 0) {
            divcnt = (uint32_t *)malloc(sizeof(uint32_t) * needed);
        } else {
            divcnt = (uint32_t *)realloc(divcnt, sizeof(uint32_t) * needed);
        }
        divcnt_sz = needed;
    }

    /* Compute divisor counts up to n if necessary. */
    if (div_max_computed < n) {
        /* Zero counts for indices 1..n. (Index 0 is unused.) */
        for (size_t i = 1; i <= n; ++i) {
            divcnt[i] = 0;
        }
        /* Simple sieve to count divisors. */
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = i; j <= n; j += i) {
                ++divcnt[j];
            }
        }
        div_max_computed = n;
    }

    /* Set the first term a(0) = 1 if not yet done. */
    if (computed_max == 0) {
        cache[0] = 1;
        computed_max = 0;
    }

    /* Compute new sequence terms up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            /* coefficient = k * d(k) */
            seq_t coeff = (seq_t)k * (seq_t)divcnt[(size_t)k];
            sum += coeff * cache[(size_t)(i - k)];
        }
        cache[(size_t)i] = sum;
    }
    computed_max = n;

    return cache[(size_t)n];
}
