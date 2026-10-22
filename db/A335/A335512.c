/*
 * A335512  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,1,1).
 * https://oeis.org/A335512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   has_triplet
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_triplet.h"

/* Sequence A335512:
   a(n) is the n‑th integer k such that the k‑th composition in
   standard order (A066099) has some part occurring at least three times.
   The implementation scans k = 1, 2, 3, …, keeping the first n such indices. */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* undefined for n = 0 (offset 1) */
    }

    /* Cache of qualifying indices. */
    static idx_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (has_triplet(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                idx_t *tmp = (idx_t *)malloc(new_cap * sizeof(idx_t));
                if (!tmp) {
                    return 0;   /* allocation failed */
                }
                if (cache) {
                    for (size_t i = 0; i < cache_len; ++i) {
                        tmp[i] = cache[i];
                    }
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return (seq_t)cache[n - 1];
}
