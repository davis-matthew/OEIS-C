/*
 * A277165  Lexicographically least sequence of nonnegative integers that
 *          avoids 27/23-powers.
 * https://oeis.org/A277165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9537, u16 n <= 9537, u32 n <= 9537, u64 n <= 9537,
 *            u128 n <= 9537, big any n
 * Helpers:   forbids
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically least sequence over {0,1,2} avoiding 27/23‑powers. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/forbids.h"

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure the cache has space for indices up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;          /* realloc handles NULL as malloc */
        cache_cap = new_cap;
    }

    /* Generate sequence up to the requested index if necessary. */
    while (cache_len <= (size_t)n) {
        for (seq_t cand = 0; cand <= 2; ++cand) {
            if (!forbids(cache, cache_len, cand)) {
                cache[cache_len++] = cand;
                break;
            }
        }
    }

    return cache[(size_t)n];
}
