/*
 * A384259  a(n) = Product_{k=0..n-1} (n+4*k+3).
 * https://oeis.org/A384259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (n+4*k+3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = (seq_t)n + 4 * k + 3;
        res *= term;
    }
    return res;
}
