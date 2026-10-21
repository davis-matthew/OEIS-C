/*
 * A270300  Numbers which are representable as a sum of thirteen but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_no_shorter_rep
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that are representable as a sum of
   thirteen consecutive nonnegative integers but not by any
   shorter length (2‑12).  Such numbers are multiples of 13 ≥ 78,
   filtered by a simple check for shorter representations. */

#include <stdlib.h>
#include "../utils/has_no_shorter_rep.h"

seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next candidate multiple of 13 to test */
    static seq_t next_candidate = ((seq_t)13 * 6); /* start with 78 */

    if (n == 0) return 0; /* undefined for n=0 */

    /* grow cache if needed */
    if (cache_len < n) {
        /* ensure capacity at least n */
        if (cache_cap < n) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms until we have n */
        while (cache_len < n) {
            /* find next valid candidate */
            while (!has_no_shorter_rep(next_candidate))
                next_candidate += (seq_t)13;   /* next multiple of 13 */

            cache[cache_len++] = next_candidate;
            next_candidate += (seq_t)13;       /* advance to next multiple */
        }
    }
    return cache[n - 1];
}
