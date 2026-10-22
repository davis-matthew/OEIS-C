/*
 * A319948  a(n) = Product_{i=1..n} floor(3*i/2).
 * https://oeis.org/A319948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} floor(3*i/2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
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
        seq_t term = (seq_t)(((seq_t)3 * (seq_t)i) / (seq_t)2); /* floor(3*i/2) */
        prod *= term;
    }
    return prod;
}
