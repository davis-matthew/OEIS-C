/*
 * A320345  a(n) = Product_{k=0..n} (k^9 + (n-k)^9).
 * https://oeis.org/A320345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^9 + (n-k)^9)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow9(seq_t x) {
    seq_t r = 1;
    for (int i = 0; i < 9; ++i) r *= x;
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_pow = pow9((seq_t)k);
        seq_t nk_pow = pow9((seq_t)(n - k));
        result *= (k_pow + nk_pow);
    }
    return result;
}
