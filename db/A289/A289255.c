/*
 * A289255  a(n) = 4^n - 2*n - 1.
 * https://oeis.org/A289255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2*n - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute 4^n by repeated multiplication. */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;
    }
    /* Subtract 2*n + 1. */
    seq_t two_n = (seq_t)2 * (seq_t)n;
    return pow4 - two_n - (seq_t)1;
}
