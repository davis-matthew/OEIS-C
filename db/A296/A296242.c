/*
 * A296242  Slideable numbers: each digit d can be displaced by d positions
 *          either to the left or to the right, without creating a hole or an
 *          "overlap".
 * https://oeis.org/A296242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1537, u32 n <= 1933, u64 n <= 1933,
 *            u128 n <= 1933, big any n
 * Helpers:   is_slideable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296242: slideable numbers
 * A slideable number is a decimal integer where each digit d can be displaced
 * by exactly d positions either to the left or to the right, without
 * creating any holes or overlaps.  The resulting positions of the digits
 * must form a contiguous block of slots.
 *
 * The sequence starts with: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 20, 22, 24, ...
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_slideable.h"

/* Maximum cache size – sufficient for indices up to about 1e5 */
#define MAX_CACHE 200000

/* Internal cache of computed terms */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t next_candidate = 0;   /* next integer to test */

/* ------------------------------------------------------------ */
/* Test whether a given number is slideable                     */

/* ------------------------------------------------------------ */
/* External entry point: a(n)                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n && next_candidate < (idx_t)MAX_CACHE) {
        seq_t cur = (seq_t)next_candidate;
        if (is_slideable(cur))
            cache[cache_len++] = cur;
        ++next_candidate;
    }

    /* In the unlikely event that more terms are needed than cache size */
    /* (which should not happen for the intended use), fall back to
       brute force without caching. */
    if (cache_len < (size_t)n) {
        idx_t cand = next_candidate;
        while (cache_len < (size_t)n) {
            seq_t cur = (seq_t)cand;
            if (is_slideable(cur))
                cache[cache_len++] = cur;
            ++cand;
        }
        next_candidate = cand;
    }

    return cache[n - 1];
}
