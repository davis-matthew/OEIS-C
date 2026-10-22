/*
 * A334136  a(n) = (n-1)*sigma(n) where sigma is the sum of divisors A000203.
 * https://oeis.org/A334136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*sigma(n) where sigma is the sum of divisors
 *            (A000203)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is 0 the sequence is undefined, but returning 0 is harmless. */
    if (n == 0) return 0;

    seq_t sigma = 0;                 /* sum of divisors of n */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i)              /* avoid adding the square root twice */
                sigma += (seq_t)j;
        }
    }
    seq_t N = (seq_t)n;
    return (N - 1) * sigma;
}
