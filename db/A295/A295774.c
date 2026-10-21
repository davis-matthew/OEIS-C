/*
 * A295774  a(n) is the minimum size of a restricted planar additive basis for
 *          the square [0,2n]^2.
 * https://oeis.org/A295774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 if n == 0, otherwise 4*n
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;
    return (seq_t)n * (seq_t)4;
}
