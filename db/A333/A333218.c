/*
 * A333218  Numbers k such that the k-th composition in standard order is a
 *          permutation (of an initial interval).
 * https://oeis.org/A333218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_perm_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by searching for k such that the k‑th composition in standard
   order is a permutation of an initial interval. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_perm_k.h"

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_k = 0;

    /* Initialise cache with first term 0 if empty. */
    if (cache_size == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_size = 1;
        last_k = 0;
    }

    /* Grow cache until it holds at least n terms. */
    while (cache_size < n) {
        idx_t k = last_k + 1;
        while (!is_perm_k(k)) ++k;
        /* Add new term. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)k;
        last_k = k;
    }

    return cache[(size_t)n - 1];
}
