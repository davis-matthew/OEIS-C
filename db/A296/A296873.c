/*
 * A296873  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose base‑7 digits have the same
 *            number of pits and peaks. A pit at index i satisfies d(i-1) >
 *            d(i) < d(i+1); a peak satisfies d(i-1) < d(i) > d(i+1)
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/balanced_base7.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cap   = 0;      /* current allocation size */
    static size_t size  = 0;      /* number of terms stored */
    static uint64_t cur  = 0;     /* last integer examined */

    /* Grow the cache until it contains at least n terms. */
    while (size < (size_t)n) {
        ++cur;                           /* next candidate integer   */
        if (balanced_base7(cur)) {
            /* Allocate / expand the cache if needed. */
            if (size == cap) {
                size_t new_cap = cap ? 2 * cap : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* out of memory */
                cache = tmp;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)cur;   /* store the term      */
        }
    }

    return cache[n - 1];
}
