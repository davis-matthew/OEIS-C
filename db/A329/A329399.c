/*
 * A329399  Numbers whose reversed binary expansion has uniform Lyndon
 *          factorization.
 * https://oeis.org/A329399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_uniform_lyndon_reversed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the nth term of OEIS A329399.
 * For n ≥ 1, a(n) is the nth integer whose reversed binary
 * representation has a uniform Lyndon factorisation.
 * The function searches for such integers in ascending order,
 * memoising the results for subsequent calls.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_uniform_lyndon_reversed.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_num = 0;

    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        idx_t cand = last_num + 1;
        last_num = cand;
        if (is_uniform_lyndon_reversed(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure; return 0 as a fallback */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
        /* Increment cand for the next iteration; already done at loop end */
        last_num = cand;
    }

    return cache[n - 1];
}
