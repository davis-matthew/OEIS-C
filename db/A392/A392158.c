/*
 * A392158  For each starting value n, the number of distinct terms in the
 *          sequence obtained by repeatedly doubling and then deleting
 *          repeated digits (keeping the first occurrence) until a term
 *          repeats.
 * https://oeis.org/A392158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct terms before first repetition after
 *            repeatedly doubling and then deleting repeated digits (keeping
 *            only the first occurrence)
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   double_and_filter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/double_and_filter.h"

#define MAX_TERMS 500            /* sufficient for all expected inputs */

/* --------------------------------------------------------------------- */
/*  Return the next term in the sequence: double the value and delete     */
/*  any repeated decimal digits, keeping only the first occurrence.       */

/* --------------------------------------------------------------------- */
/*  Main function to compute a(n).  N >= 1 for the OEIS offset 1        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;
    seq_t seen[MAX_TERMS];
    size_t seen_len = 0;

    /* add the starting value */
    seen[seen_len++] = current;

    while (true) {
        seq_t next = double_and_filter(current);

        /* check if next has been seen before */
        bool found = false;
        for (size_t i = 0; i < seen_len; ++i) {
            if (seen[i] == next) {
                found = true;
                break;
            }
        }

        if (found) {
            /* number of distinct terms = seen_len */
            return (seq_t)seen_len;
        }

        /* add next to the list of seen terms */
        if (seen_len == MAX_TERMS) {
            /* This should never happen for the intended range of n. */
            /* If it does, stop at MAX_TERMS to avoid overflow. */
            return (seq_t)MAX_TERMS;
        }
        seen[seen_len++] = next;
        current = next;
    }
}
