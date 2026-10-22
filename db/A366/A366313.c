/*
 * A366313  a(n) = Product_{k=0..2*n} (n^2 + k^2).
 * https://oeis.org/A366313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..2*n} (n^2 + k^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t nsq = (seq_t)n * (seq_t)n;
    for (idx_t k = 0; k <= 2 * n; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        prod *= (nsq + ksq);
    }
    return prod;
}
