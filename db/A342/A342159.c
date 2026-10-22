/*
 * A342159  Number of words of length n, over the alphabet {a,b,c}, which have
 *          an odd number of a's and the number of b's plus the number of c's
 *          is less than or equal to 3.
 * https://oeis.org/A342159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 14*n)/3 - 4*n^2 for n even; a(n) = 1 - 2*n +
 *            2*n^2 for n odd
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0) {
        /* Even n:  (4*n^3 + 14*n)/3 - 4*n^2 */
        seq_t num = (seq_t)4 * nn * nn * nn + (seq_t)14 * nn;
        num /= (seq_t)3;
        return num - (seq_t)4 * nn * nn;
    } else {
        /* Odd n:   1 - 2*n + 2*n^2 */
        return (seq_t)2 * nn * nn + (seq_t)1 - (seq_t)2 * nn;
    }
}
