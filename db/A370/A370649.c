/*
 * A370649  Dimension of space of equivariant linear maps from R^{n^3} to
 *          R^{n^3} under diagonal action of {-1, 1}^n.
 * https://oeis.org/A370649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^3 - 30*n^2 + 16*n
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 659, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n0 = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = n0 * n0;           /* n^2   */
    seq_t n3 = n2 * n0;           /* n^3   */
    return (seq_t)15 * n3
         - (seq_t)30 * n2
         + (seq_t)16 * n0;
}
