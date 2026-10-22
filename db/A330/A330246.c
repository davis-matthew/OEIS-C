/*
 * A330246  a(n) = 4^(n+1) + (4^n-1)/3.
 * https://oeis.org/A330246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+1, a(0)=4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)4;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)4 + (seq_t)1;
    }
    return val;
}
