/*
 * A383368  Number of intercalates in pine Latin squares of order 2n.
 * https://oeis.org/A383368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 if n is odd; a(n) = n^2 + n^3 if n is even (n counted
 *            from 1). This matches OEIS A383368
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* n as seq_t for arithmetic */
    seq_t nn2 = nn * nn;          /* n^2 */
    seq_t nn3 = nn2 * nn;         /* n^3 */
    if ((n & 1) == 0) {           /* even n */
        return nn2 + nn3;         /* n^2 + n^3 */
    } else {                      /* odd n */
        return nn3;               /* n^3 */
    }
}
