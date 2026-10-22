/*
 * A337955  Number of achiral colorings of the 16 tetrahedral facets of a
 *          hyperoctahedron or of the 16 vertices of a tesseract.
 * https://oeis.org/A337955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 * (3*n^8 + 5*n^4 + 12*n^2 + 28) / 48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    seq_t nn = (seq_t)n;           /* n as seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n8 = n4 * n4;           /* n^8 */
    seq_t sum = (seq_t)3 * n8      /* 3*n^8 */
             + (seq_t)5 * n4      /* 5*n^4 */
             + (seq_t)12 * n2     /* 12*n^2 */
             + (seq_t)28;         /* +28 */
    seq_t prod = n4 * sum;         /* n^4 * sum */
    return prod / (seq_t)48;       /* divide by 48 (exact) */
}
