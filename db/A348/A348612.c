/*
 * A348612  Numbers k such that the k-th composition in standard order is not
 *          an anti-run, i.e., has adjacent equal parts.
 * https://oeis.org/A348612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   has_adjacent_equal_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant: a(n) = the n‑th integer k such that the k‑th
 * composition in standard order has two consecutive equal parts.
 * The function enumerates compositions by interpreting the binary
 * representation of k (reversed) and checks for adjacent equal
 * differences between successive set-bit positions. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/has_adjacent_equal_parts.h"

/* The a(n) function as required.  It memoises results up to the
 * largest n requested so far and continues enumeration from the
 * last k processed, allowing arbitrary call order. */
seq_t a(idx_t n)
{
    static idx_t current_k = 0;      /* last processed k  (starts from 0) */
    static seq_t *cache = NULL;      /* array of found indices */
    static size_t cache_len = 0;     /* number of values currently cached */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Grow cache as needed.  Here we use plain malloc/realloc for
     * simplicity; with built‑in integer seq_t this is sufficient. */
    if (cache_len < n) {
        /* Ensure enough capacity for new elements. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Continue enumeration until we have n values. */
        while (cache_len < n) {
            current_k++;                    /* next candidate k */
            if (has_adjacent_equal_parts(current_k)) {
                /* Store the qualifying k.  The cast works for
                 * integer seq_t types; for arbitrary‑precision
                 * types the compiler will use the appropriate
                 * constructor. */
                cache[cache_len++] = current_k;
            }
        }
    }

    return cache[n - 1];  /* a is 1‑indexed */
}
