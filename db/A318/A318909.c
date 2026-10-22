/*
 * A318909  a(n) = Product_{1<=x<=n, n|(x^2-1)} x.
 * https://oeis.org/A318909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Product{1 <= x <= n, n|(x^2-1)} x
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 55, u64 n <= 119,
 *            u128 n <= 839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t sn = (seq_t)n;
    for (idx_t x = 1; x <= n; ++x) {
        seq_t xx = (seq_t)x * (seq_t)x;
        if ((xx - 1) % sn == 0) {
            prod *= (seq_t)x;
        }
    }
    return prod;
}
