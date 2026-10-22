/*
 * A397071  Number of true statements (x op y) = z, where 0 <= x, y, z <= n,
 *          op is in {+,-,*,/}.
 * https://oeis.org/A397071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 6*n + 3 + 2*Sum_{k=1..n} floor(n/k)
 * Range:     u8 n <= 11, u16 n <= 247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        sum += (seq_t)(n / k);
    }
    return (seq_t)n * (seq_t)n + (seq_t)6 * (seq_t)n + 3 + 2 * sum;
}
