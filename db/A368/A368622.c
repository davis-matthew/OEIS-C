/*
 * A368622  a(n) = Product_{j=1..n, k=1..n} (j^2 + k^2 + n^2).
 * https://oeis.org/A368622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n, k=1..n} (j^2 + k^2 + n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t n2 = (seq_t)n * (seq_t)n;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t j2 = (seq_t)j * (seq_t)j;
        seq_t j2n = j2 + n2;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t k2 = (seq_t)k * (seq_t)k;
            seq_t term = j2n + k2;
            prod *= term;
        }
    }
    return prod;
}
