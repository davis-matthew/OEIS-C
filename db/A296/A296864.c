/*
 * A296864  Numbers whose base-4 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_peaks_pits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A296864: the n-th positive integer whose base‑4 digits contain
   the same number of pits and peaks.
   A pit is an index i with d(i‑1) > d(i) < d(i+1); a peak is d(i‑1) < d(i) > d(i+1).
   The function searches integers in ascending order, storing results for
   later calls.  Complexity is O(a(n)) where a(n) is the n-th term.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/equal_peaks_pits.h"

seq_t a(idx_t n)
{
    static idx_t next_val = 1;          /* next integer to examine          */
    static seq_t *cache = NULL;         /* cached sequence values           */
    static size_t cache_len = 0;        /* number of values stored          */
    static size_t cache_cap = 0;        /* capacity of allocated array      */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (equal_peaks_pits(next_val)) {
            if (cache_len == cache_cap) {   /* grow the cache if needed   */
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failure handled   */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_val;
        }
        next_val++;
    }
    return cache[n-1];
}
