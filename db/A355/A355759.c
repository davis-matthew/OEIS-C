/*
 * A355759  Sums of the first ceiling((n+1)/2) entries on the diagonals of a
 *          square spiral with a starting value of 1 in the center, where the
 *          diagonal and the antidiagonal are used alternately.
 * https://oeis.org/A355759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24 + 20*n + 6*n^2 + n^3) / 24 (n even) a(n) = (12 + 17*n
 *            + 6*n^2 + n^3) / 24 (n odd, n mod 4 == 3) a(n) = (17*n + 6*n^2 +
 *            n^3) / 24 (n odd, n mod 4 == 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1649, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    if ((n & 1) == 0) {
        return (24 + 20 * nn + 6 * nn2 + nn3) / 24;
    } else {
        if ((n & 3) == 1) {
            return (17 * nn + 6 * nn2 + nn3) / 24;
        } else { /* (n & 3) == 3 */
            return (12 + 17 * nn + 6 * nn2 + nn3) / 24;
        }
    }
}
