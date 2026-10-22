/*
 * A371396  Maximum number of vertices of a chordal ring graph with diameter
 *          n.
 * https://oeis.org/A371396
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (3*n^2+1)/2 if n is odd; a(n) = (3*n^2)/2 - n for even
 *            n>2; a(2) = 6
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* below offset: undefined but harmless */
    if (n == 2) return (seq_t)6;

    seq_t nn = (seq_t)n;
    if (n & 1ULL) {                /* odd n */
        return ((seq_t)3 * nn * nn + 1) / 2;
    } else {                       /* even n > 2 */
        return ((seq_t)3 * nn * nn) / 2 - nn;
    }
}
