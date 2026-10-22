/*
 * A338041  Draw n rays from each of two distinct points in the plane; a(n) is
 *          the number of regions thus created. See Comments for details.
 * https://oeis.org/A338041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 8*n – 5)/4 if n is odd; (n^2 + 2*n)/4 if n is even
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, return 0 for safety */
    seq_t tn = (seq_t)n;
    if (n & 1ULL) {                 /* n odd */
        return (tn * tn + (seq_t)8 * tn - 5) / 4;
    } else {                        /* n even */
        return (tn * tn + (seq_t)2 * tn) / 4;
    }
}
