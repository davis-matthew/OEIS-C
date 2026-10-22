/*
 * A325657  a(n) = (1/2)*(-1 + (-1)^n)*(n-1) + n^2.
 * https://oeis.org/A325657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 if n even, a(n) = n^2 - n + 1 if n odd
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        return nn * nn;
    } else {
        return nn * nn - nn + 1;
    }
}
