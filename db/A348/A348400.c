/*
 * A348400  a(1) = 1; a(n+1) = a(n) + n if the digit sum of a(n) is already in
 *          the sequence, otherwise a(n+1) = digitsum(a(n)).
 * https://oeis.org/A348400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1;
   a(n+1) = a(n) + n          if digit_sum(a(n)) is already in the sequence,
   a(n+1) = digit_sum(a(n))   otherwise.
   Indices are 1‑based. */

/* Standard headers and the digit_sum utility. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Memoisation buffer.
       seq[0] is unused to keep the logic 1‑based. */
    static seq_t *seq = NULL;
    static idx_t  len = 0;   /* Highest index computed so far (≥0). */
    static idx_t  cap = 0;   /* Current capacity (maximum index that can be stored). */

    /* ---------- Initialise on first call ---------- */
    if (len == 0) {
        cap = 1;
        seq = (seq_t *)realloc(seq, (size_t)(cap + 1) * sizeof(seq_t));
        seq[1] = (seq_t)1;   /* a(1) = 1 */
        len = 1;
    }

    /* If the requested index is already memoised, return it. */
    if (n <= len) {
        return seq[n];
    }

    /* ---------- Extend the cache up to index `n` ---------- */
    while (len < n) {
        seq_t prev  = seq[len];
        idx_t  prev_idx = len;               /* current index (n in the recurrence) */

        /* Digit sum of the current term. */
        seq_t d = digit_sum(prev);

        /* Determine whether this digit sum has already appeared. */
        bool seen = false;
        for (idx_t i = 1; i <= len; ++i) {
            if (seq[i] == d) {
                seen = true;
                break;
            }
        }

        /* Compute the next term according to the recurrence. */
        seq_t next = seen ? prev + (seq_t)(prev_idx) : d;

        /* Advance the index. */
        len++;

        /* Grow the storage if necessary. */
        if (len > cap) {
            cap = cap ? cap * 2 : 1;
            seq = (seq_t *)realloc(seq, (size_t)(cap + 1) * sizeof(seq_t));
        }

        /* Store the newly computed term. */
        seq[len] = next;
    }

    return seq[n];
}
