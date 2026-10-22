/*
 * A384257  a(n) = Product_{k=0..n-1} (n+3*k+2).
 * https://oeis.org/A384257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (n + 3*k + 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k) {
        prod *= (seq_t)(n + 3 * k + 2);
    }
    return prod;
}
