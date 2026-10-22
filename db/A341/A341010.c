/*
 * A341010  Numbers whose sum of even digits and sum of odd digits differ by
 *          9.
 * https://oeis.org/A341010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3789, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   diff9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose sum of even digits and sum of odd digits differ by 9.
   Computed by scanning consecutive integers and selecting those that satisfy
   the property. This is a direct search implementation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/diff9.h"

/* a(n) returns the n-th term (1‑based) of A341010. */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Offset is 1; behaviour for n < 1 is undefined but return 0. */
        return 0;
    }

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t current = 1;                 /* next integer to test */

    /* Lazy allocation of cache. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure */
    }

    /* Extend cache until it contains at least n terms. */
    while (cache_len < n) {
        if (diff9(current)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    return 0;   /* allocation failure */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current;
        }
        current++;
    }

    return cache[n - 1];
}
