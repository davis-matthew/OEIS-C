/*
 * A398916  a(n) is the Grundy value of "Bit Deletion" game at state n (see
 *          comments).
 * https://oeis.org/A398916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = (L odd? (m odd?3:1) : (m odd?2:0)) where L is the binary
 *            length of n and m is the largest index of zero‑runs (between
 *            successive 1s) such that all run lengths up to that index are
 *            odd
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Grundy value of the “Bit Deletion” game (seq. A398916). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute bit length L of n. */
    int L = 0;
    idx_t tmp = n;
    while (tmp) {
        L++;
        tmp >>= 1;
    }

    /* Scan zero runs to find m. */
    int runIndex = 1;    /* index of the current zero‑run (after the first 1) */
    int zeros = 0;       /* length of the current zero‑run being counted */
    bool allOdd = true;  /* true iff all zero‑runs seen so far are odd */
    int m = 0;           /* largest index whose preceding runs are all odd */

    for (int bit = L - 2; bit >= 0; --bit) {
        if ((n >> bit) & 1ULL) {          /* encountered a '1' */
            /* Finished a zero‑run of length 'zeros'. */
            if (allOdd && (zeros & 1)) {
                m = runIndex;
            } else if (!(zeros & 1)) {
                allOdd = false;
            }
            runIndex++;   /* ready for the next zero‑run */
            zeros = 0;
        } else { /* bit is '0' */
            zeros++;
        }
    }
    /* Handle trailing zeros after the last 1. */
    if (allOdd && (zeros & 1)) {
        m = runIndex;
    } else {
        allOdd = false;
    }

    /* Determine Grundy value based on L and m. */
    if (L & 1)          /* L is odd */
        return (seq_t)((m & 1) ? 3U : 1U);
    else                /* L is even */
        return (seq_t)((m & 1) ? 2U : 0U);
}
