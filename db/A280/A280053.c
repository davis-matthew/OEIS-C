/*
 * A280053  "Nachos" sequence based on squares.
 * https://oeis.org/A280053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of phases required to empty a pile of n nachos by
 *            repeatedly subtracting successive squares 1²,2²,3²,…. In each
 *            phase, the largest sum of consecutive squares not exceeding the
 *            current pile size is removed; the process repeats until the pile
 *            is empty. The k‑th phase removes T(k)=k(k+...
 * Range:     u8 n <= 1376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the number of phases needed for a pile of n nachos. */
seq_t a(idx_t n)
{
    seq_t remaining = (seq_t)n;   /* remaining nachos */
    seq_t phases     = 0;         /* phase counter */

    while (remaining > 0) {
        seq_t sum = 0;            /* sum of squares removed in this phase */
        idx_t  i   = 1;           /* current square index */

        while (1) {
            seq_t sq = (seq_t)i * (seq_t)i;     /* next square */
            /* Stop if adding the next square would exceed the remaining pile. */
            if (sq > remaining || sum > remaining - sq)
                break;
            sum += sq;
            ++i;
        }

        remaining -= sum;
        ++phases;
    }

    return phases;
}
