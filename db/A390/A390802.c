/*
 * A390802  The sum of the integers k from 1 to n such that gcd(n, k) is odd.
 * https://oeis.org/A390802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 if n is odd, n*n/4 if n is even
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    if ((n & 1u) == 0u)          /* n even */
        return N * N / 4;
    else                         /* n odd  */
        return N * (N + 1) / 2;
}
