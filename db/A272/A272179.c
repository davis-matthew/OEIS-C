/*
 * A272179  a(n) = Product_{k=0..n} (n^2 - k).
 * https://oeis.org/A272179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n} (n^2 - k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    for (idx_t k = 0; k <= n; ++k)
    {
        res *= (nsq - (seq_t)k);
    }
    return res;
}
