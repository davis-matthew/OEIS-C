/*
 * A296882  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_pits_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑10 digits have an equal number of pits
   (local minima) and peaks (local maxima).  The routine generates
   terms by testing successive integers and caching the results. */

/* ------------------------------------------------------------ */
/* Standard headers required for C11 and C++17 compatibility   */
/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_pits_peaks.h"

/* ------------------------------------------------------------ */
/* Helper: test whether a number has the same count of pits and
   peaks in its decimal representation.                                 */

/* ------------------------------------------------------------ */
/* Public function: a(idx_t n) – returns the nth element of the
   sequence (1‑based).                                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static storage for discovered terms and the next integer to
       test.  The cache is lazily expanded on demand. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;

    /* Expand cache until it holds at least n terms. */
    while (cache_len < n) {
        if (has_equal_pits_peaks(next)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)         /* allocation failure – return a safe value */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        ++next;
    }
    return cache[n - 1];
}
