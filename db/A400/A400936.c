/*
 * A400936  Product of the distinct entries in the central row and column of
 *          the (2*n + 1) X (2*n + 1) square spiral.
 * https://oeis.org/A400936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Product_{k = 1..n}(((4*k^2 + 1)^2 - k^2)*((4*k^2 + 1)^2 -
 *            9*k^2))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;
        seq_t t = (seq_t)4 * kk + 1;
        seq_t t_sq = t * t;
        seq_t f1 = t_sq - kk;
        seq_t f2 = t_sq - (seq_t)9 * kk;
        prod *= f1;
        prod *= f2;
    }
    return prod;
}
