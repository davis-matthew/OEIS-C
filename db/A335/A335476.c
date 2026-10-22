/*
 * A335476  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,1,2).
 * https://oeis.org/A335476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   matches_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by scanning the compositions in standard order
   and returning the n-th number k whose k-th composition
   contains a subsequence that matches the pattern (1,1,2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/matches_pattern.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Static cache of found values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;   /* last k processed */

    /* Ensure cache can hold at least 'n' results */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search further until we have n results */
    while (cache_len < (size_t)n) {
        ++last_k;
        /* Build composition of last_k */
        idx_t tmp = last_k;
        size_t pos[64];
        size_t poscnt = 0;
        size_t bit = 1;
        while (tmp) {
            if (tmp & 1ULL) pos[poscnt++] = bit;
            tmp >>= 1;
            ++bit;
        }
        /* parts array of the composition in reverse order */
        seq_t parts[64];
        idx_t prev = 0;
        for (size_t i = 0; i < poscnt; ++i) {
            idx_t cur = pos[i];
            idx_t diff = cur - prev;
            parts[poscnt - 1 - i] = (seq_t)diff;
            prev = cur;
        }

        if (matches_pattern(parts, poscnt))
            cache[cache_len++] = (seq_t)last_k;
    }

    return cache[n - 1];
}
