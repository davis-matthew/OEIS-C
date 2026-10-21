/*
 * A241453  a(n) = pg(3, n) * pg(4, n) * ... * pg(n, n) where pg(m, n) is the
 *          n-th m-th-order polygonal number.
 * https://oeis.org/A241453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{m=3}^{n} ((m-2)*n^2 - (m-4)*n)/2 (a(0)=a(1)=a(2)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 1;
    seq_t prod = 1;
    seq_t nsq = (seq_t)n * (seq_t)n;
    for (idx_t m = 3; m <= n; ++m) {
        seq_t m2 = (seq_t)m;
        seq_t term = ((m2 - 2) * nsq - (m2 - 4) * (seq_t)n) / 2;
        prod *= term;
    }
    return prod;
}
