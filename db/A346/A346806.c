/*
 * A346806  Numbers that are the sum of ten squares in seven or more ways.
 * https://oeis.org/A346806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_terms_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A346806 – Numbers that are the sum of ten positive squares in
   seven or more distinct unordered representations.
   The code builds the list of qualifying numbers by dynamic programming
   over all squares up to the current search limit.  It expands the limit
   as needed until it has found at least n terms.
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/generate_terms_up_to.h"

seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static seq_t *found      = NULL;     /* array of terms */
    static size_t found_len  = 0;        /* number of terms */
    static idx_t  current_lim = 0;       /* current search limit */
    static int    initialized  = 0;      /* flag for first run */

    /* If we don't yet have n terms, grow the search window. */
    if (!initialized || found_len < n) {
        idx_t new_lim = current_lim ? current_lim * 2 : 2000;

        /* Keep extending until we have enough terms. */
        while (found_len < n) {
            seq_t *terms = NULL;
            size_t len   = 0;
            generate_terms_up_to(new_lim, &terms, &len);

            /* Replace the old list with the new one. */
            if (found) free(found);
            found      = terms;
            found_len  = len;
            current_lim = new_lim;

            if (found_len >= n) break;
            /* Prevent integer overflow on the next doubling. */
            if (new_lim > SIZE_MAX / 2) break;
            new_lim *= 2;
        }
        initialized = 1;
    }

    /* n is 1‑based: a(1) is the first term. */
    return found[n - 1];
}
