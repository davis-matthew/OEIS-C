/*
 * A384303  a(n) = Product_{k=0..2*n-1} (3*n+k+1).
 * https://oeis.org/A384303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)!/(3*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 3 * n + 1; i <= 5 * n; ++i)
        res *= (seq_t)i;
    return res;
}
