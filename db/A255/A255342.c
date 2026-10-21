/*
 * A255342  Numbers n such that there are exactly two 1's in their factorial
 *          base representation (A007623).
 * https://oeis.org/A255342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search numbers with exactly two 1's in their factorial base
 *            representation
 * Range:     u8 n <= 90, u16 n <= 9516, u32 n <= 9516, u64 n <= 9516,
 *            u128 n <= 9516, big any n
 * Helpers:   count_ones_factorial_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_ones_factorial_base.h"

/* a returns the n-th member of A255342. */
seq_t a(idx_t n) {
    /* Static cache to answer any sequence of calls in O(1) after the
       initial construction of the required prefix. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t last_checked = 0;   /* last candidate examined */

    if (n == 0) return 0;           /* undefined, but guard against underflow */

    if ((idx_t)cache_size >= n) {
        return cache[n - 1];         /* already computed */
    }

    /* Expand cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        ++last_checked;                     /* next integer to examine */

        if (count_ones_factorial_base(last_checked) == 2) {
            /* Grow cache if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    return cache[n - 1];
}
