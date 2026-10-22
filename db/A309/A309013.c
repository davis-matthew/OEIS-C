/*
 * A309013  Lexicographically earliest squarefree sequence of nonnegative
 *          integers whose even and odd bisections are also squarefree.
 * https://oeis.org/A309013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – lexicographically earliest squarefree sequence of *
 *            nonnegative integers whose subsequences of even and odd indices
 *            * (1‑based) are also squarefree. Computed by greedy construction
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   suffix_has_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/suffix_has_square.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t *even = NULL;
    static size_t even_len = 0, even_cap = 0;
    static seq_t *odd = NULL;
    static size_t odd_len = 0, odd_cap = 0;

    /* build sequence up to required position n (1‑based) */
    while (cache_len < (size_t)n) {
        size_t pos = cache_len + 1;          /* position to fill, 1‑based */
        seq_t val;
        for (val = 0; ; ++val) {
            /* ensure capacity for cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) exit(EXIT_FAILURE);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len] = val;  /* temporary candidate */

            /* check square‑free property of the full sequence */
            if (!suffix_has_square(cache, cache_len + 1)) {
                bool ok = true;
                if (pos % 2 == 0) {
                    /* ensure capacity for even subsequence */
                    if (even_len == even_cap) {
                        size_t new_cap = even_cap ? even_cap * 2 : 4;
                        seq_t *new_arr = (seq_t *)realloc(even, new_cap * sizeof(seq_t));
                        if (!new_arr) exit(EXIT_FAILURE);
                        even = new_arr;
                        even_cap = new_cap;
                    }
                    even[even_len] = val;
                    if (suffix_has_square(even, even_len + 1)) ok = false;
                } else {
                    /* ensure capacity for odd subsequence */
                    if (odd_len == odd_cap) {
                        size_t new_cap = odd_cap ? odd_cap * 2 : 4;
                        seq_t *new_arr = (seq_t *)realloc(odd, new_cap * sizeof(seq_t));
                        if (!new_arr) exit(EXIT_FAILURE);
                        odd = new_arr;
                        odd_cap = new_cap;
                    }
                    odd[odd_len] = val;
                    if (suffix_has_square(odd, odd_len + 1)) ok = false;
                }

                if (ok) {
                    /* commit the chosen value */
                    ++cache_len;
                    if (pos % 2 == 0) ++even_len;
                    else ++odd_len;
                    break;  /* exit candidate loop */
                }
                /* candidate rejected – lengths stay unchanged,
                   next loop iteration writes new val into cache[cache_len]   */
            }
        }
    }

    return cache[(size_t)n - 1];
}
