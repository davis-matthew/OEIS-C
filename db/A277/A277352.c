/*
 * A277352  a(n) = Product_{k=1..n} (2*k^2+1).
 * https://oeis.org/A277352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} (2*k^2+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;       /* start with a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t term = 2 * k * k + 1;
        prod = prod * (seq_t)term;
    }
    return prod;
}
