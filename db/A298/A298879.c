/*
 * A298879  Numbers whose square is not odious.
 * https://oeis.org/A298879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th non‑negative integer m such that the binary *
 *            popcount of m*m is even (i.e. m*m is not odious)
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the n‑th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    idx_t m = 0;        /* current candidate integer */
    idx_t count = 0;    /* number of terms found so far */

    while (count < n) {
        /* Square the candidate. */
        seq_t sq = (seq_t)m * (seq_t)m;

        /* Compute parity of the binary popcount of sq. */
        bool odd_parity = false;  /* true  → odd popcount
                                     false → even popcount */
        seq_t t = sq;
        while (t) {
            odd_parity ^= (t & 1) != 0;
            t >>= 1;
        }

        /* If the popcount is even, we have a new term. */
        if (!odd_parity) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }

    /* Unreachable for valid inputs (n ≥ 1). */
    return (seq_t)0;
}
