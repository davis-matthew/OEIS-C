/*
 * A301616  a(n) = Product_{k=1..n} (k^2+(n-k+1)^2).
 * https://oeis.org/A301616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} (k^2 + (n-k+1)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t t   = (seq_t)(n - k + 1);
        seq_t tsq = t * t;
        seq_t term = ksq + tsq;
        prod = prod * term;
    }
    return prod;
}
