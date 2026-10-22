/*
 * A306760  a(n) = Product_{i=1..n, j=1..n} (i*j + 1).
 * https://oeis.org/A306760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} ∏_{j=1..n} (i*j + 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = 1; j <= n; ++j)
        {
            seq_t term = (seq_t)i * (seq_t)j + (seq_t)1;
            prod *= term;
        }
    }
    return prod;
}
