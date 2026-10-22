/*
 * A380416  Number of corona for a diamond of edge n with diamonds of side 1.
 * https://oeis.org/A380416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^4 + 8*n^3 + 24*n^2 + 32*n + 18
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */
    /* compute the polynomial */
    return n4
         + (seq_t)8 * n3
         + (seq_t)24 * n2
         + (seq_t)32 * N
         + (seq_t)18;
}
