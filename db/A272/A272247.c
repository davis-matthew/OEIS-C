/*
 * A272247  a(n) = Product_{k=0..n} (n^4 + k^4).
 * https://oeis.org/A272247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=0..n} (n^4 + k^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t n4 = (seq_t)n * n * n * n;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t k4 = (seq_t)k * k * k * k;
        prod *= (n4 + k4);
    }
    return prod;
}
