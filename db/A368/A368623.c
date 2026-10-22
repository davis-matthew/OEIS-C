/*
 * A368623  a(n) = Product_{k=1..n} (k^2 + 2*n^2).
 * https://oeis.org/A368623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (k^2 + 2*n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Precompute 2*n^2 once to avoid recomputation in the loop. */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t two_n2 = (seq_t)2 * n_sq;

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        prod *= (k_sq + two_n2);
    }
    return prod;
}
