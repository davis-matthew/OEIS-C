/*
 * A375841  a(n) = Product_{k=0..n} (k^4 + n).
 * https://oeis.org/A375841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (k^4 + n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t k2 = kk * kk;
        seq_t k4 = k2 * k2;
        seq_t term = k4 + (seq_t)n;
        prod *= term;
    }
    return prod;
}
