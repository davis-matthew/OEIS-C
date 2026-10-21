/*
 * A296711  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   member_b9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine outputs the n‑th term of OEIS A296711.
   It searches for numbers whose base‑9 digits have fewer rises
   than falls, building a cache so calls in any order are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/member_b9.h"

seq_t a(idx_t n)
{
    /* Simple cache: array of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;          /* start search at 1 */

    /* Return cached value if possible. */
    if ((idx_t)n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next term in the sequence. */
        while (!member_b9(next_candidate))
            ++next_candidate;

        /* Ensure we have space for the new element. */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
            /* C++ and C both accept a C‑style cast from void* to
               the required type. */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)   /* allocation failed – abort */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
