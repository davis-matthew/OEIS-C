/*
 * A399561  G.f. A(x) satisfies: A(x) = (1 - x^2 + x^2 * A(2*x)) / (1 - x -
 *          x^3).
 * https://oeis.org/A399561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399561
   a(0) = a(1) = a(2) = 1;
   a(n) = a(n-1) + 2^(n-2) * a(n-2) + a(n-3)   for n ≥ 3 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence terms and powers of two */
    static seq_t *cache     = NULL;
    static seq_t *p2_cache  = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache holds terms up to n */
    if ((size_t)n + 1 > cache_cap) {
        /* allocate or grow the term cache */
        if (!cache) {
            cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failed */
        } else if ((size_t)n + 1 > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache,
                          ((size_t)n + 1) * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
        }

        /* allocate or grow the power‑of‑two cache */
        if (!p2_cache) {
            p2_cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
            if (!p2_cache) return (seq_t)0;
        } else if ((size_t)n + 1 > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(p2_cache,
                          ((size_t)n + 1) * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            p2_cache = tmp;
        }

        /* fill in terms from the previous size up to n */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            /* compute 2^i in the power cache */
            if (i == 0) {
                p2_cache[0] = (seq_t)1;
            } else {
                p2_cache[i] = seq_mul(p2_cache[i-1], (seq_t)2);
            }

            /* initial values */
            if (i <= 2) {
                cache[i] = (seq_t)1;
            } else {
                seq_t term = cache[i-1];
                term = term + seq_mul(p2_cache[i-2], cache[i-2]);
                term = term + cache[i-3];
                cache[i] = term;
            }
        }
        cache_cap = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
