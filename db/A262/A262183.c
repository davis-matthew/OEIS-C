/*
 * A262183  a(0) = 0, a(n) = 10*a(n-1) + n*(n+1)*(n+2)/6.
 * https://oeis.org/A262183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0)=0, a(n)=10*a(n-1)+n*(n+1)*(n+2)/6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = ((seq_t)i * ((seq_t)i + 1) * ((seq_t)i + 2)) / 6;
        res = res * 10 + term;
    }
    return res;
}
