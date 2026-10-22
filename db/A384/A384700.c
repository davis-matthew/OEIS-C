/*
 * A384700  On a 2 X n grid of vertices, draw a circle through every unordered
 *          triple of non-collinear vertices: a(n) is the number of distinct
 *          circles created.
 * https://oeis.org/A384700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^3/2 - n^2/4 - n + (n%2==1 && n>1 ? 3/4 : 0); a(1)=0
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the first term (offset 1) */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Convert index to seq_t for arithmetic */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;      /* n^2 */
    seq_t N3 = N2 * N;     /* n^3 */

    /* Even n: a = n^3/2 - n^2/4 - n */
    /* Odd n > 1: a = (2*n^3 - n^2 - 4*n + 3)/4 */
    if ((n & 1) == 0) { /* even */
        return N3 / 2 - N2 / 4 - N;
    } else {
        seq_t num = (seq_t)2 * N3 - N2 - (seq_t)4 * N + (seq_t)3;
        return num / 4;
    }
}
