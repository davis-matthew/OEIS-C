/*
 * A272180  a(n) = Product_{k=0..n} (n^2 + k).
 * https://oeis.org/A272180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=0}^{n} (n^2 + k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k)
        result *= (nsq + (seq_t)k);
    return result;
}
