/*
 * A316224  a(n) = n*(2*n + 1)*(4*n + 1).
 * https://oeis.org/A316224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n + 1)*(4*n + 1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (nn * (seq_t)2 + (seq_t)1) * (nn * (seq_t)4 + (seq_t)1);
}
