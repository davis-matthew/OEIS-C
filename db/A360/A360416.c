/*
 * A360416  a(n) = 8*n^2 - 9*n + 3.
 * https://oeis.org/A360416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 9*n + 3
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (seq_t)8 * nn * nn - (seq_t)9 * nn + (seq_t)3;
}
