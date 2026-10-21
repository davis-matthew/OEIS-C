/*
 * A272244  a(n) = Product_{k=0..n} (n^2 + k^2).
 * https://oeis.org/A272244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (n^2 + k^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t term = n_sq + k_sq;
        prod = prod * term;
    }
    return prod;
}
