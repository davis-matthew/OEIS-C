/*
 * A263430  a(n) = Product_{k=0..n} (4*k+1)^(n-k).
 * https://oeis.org/A263430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (4*k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t B = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        result *= B;
        seq_t base = (seq_t)1 + (seq_t)4 * (seq_t)(i + 1);
        B *= base;
    }
    return result;
}
