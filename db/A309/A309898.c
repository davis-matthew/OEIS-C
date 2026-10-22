/*
 * A309898  For each b = 1, 2, 3, ... numbers k = 1, 2, 3, ... are inserted
 *          into the blanks within the sequence along with k * b blanks,
 *          skipping existing terms in the sequence.
 * https://oeis.org/A309898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the value at position n when numbers are repeatedly
   inserted into the sequence according to the process described for
   OEIS sequence A309898: for each b = 1, 2, 3, ... insert the numbers
   k = 1, 2, 3, ... into successive blanks while skipping k·b blanks
   (counting only blanks, ignoring already-filled terms).  This
   implementation simulates that process until all positions up to n
   are filled. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* not used, but harmless */

    /* Allocate array for positions 1..n (index 0 unused). */
    seq_t *val = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!val)
        return (seq_t)0;          /* allocation failure → zero */

    for (idx_t i = 1; i <= n; ++i)
        val[i] = (seq_t)0;        /* blank */

    idx_t remaining = n;          /* number of still-unfilled positions */

    for (idx_t b = 1; remaining > 0; ++b) {
        idx_t k = 1;              /* current number to insert */
        idx_t start = 1;          /* next search start position */

        while (start <= n && remaining > 0) {
            /* find next blank from position 'start' */
            idx_t pos = start;
            while (pos <= n && val[pos] != (seq_t)0) ++pos;
            if (pos > n)
                break;            /* no blanks left for this b */

            /* insert k at pos */
            val[pos] = (seq_t)k;
            --remaining;

            /* skip k*b blank positions */
            idx_t skip_needed = k * b;     /* k and b are <= remaining <= n, safe in idx_t */
            idx_t p = pos + 1;
            idx_t skipped = 0;

            while (p <= n && skipped < skip_needed) {
                if (val[p] == (seq_t)0)     /* count only blanks */
                    ++skipped;
                ++p;
            }

            start = p;     /* next search starts after the skipped blanks */
            ++k;           /* next number to insert */
        }
    }

    seq_t result = val[n];
    free(val);
    return result;
}
