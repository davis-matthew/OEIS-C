/*
 * A365340  a(n) = (4*n)!/(3*n+1)!.
 * https://oeis.org/A365340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)!/(3*n+1)! = product_{k=3*n+2}^{4*n} k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;
    seq_t result = 1;
    idx_t start = 3 * n + 2;
    idx_t end   = 4 * n;
    for (idx_t i = start; i <= end; ++i)
        result *= (seq_t)i;
    return result;
}
