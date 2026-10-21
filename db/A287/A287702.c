/*
 * A287702  a(n) = (3!)^3 * [z^3] hypergeom([], [1,1], z)^n.
 * https://oeis.org/A287702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^3 - 81*n^2 + 46*n
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    return (seq_t)36 * nn3 - (seq_t)81 * nn2 + (seq_t)46 * nn;
}
