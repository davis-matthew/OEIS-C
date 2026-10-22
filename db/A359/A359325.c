/*
 * A359325  Number of divisors of 6*n-3 of form 6*k+5.
 * https://oeis.org/A359325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of (6*n-3) with d % 6 == 5
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* According to OEIS, index starts at 1.  For safety, return 0 for n==0. */
    if (n == 0) return 0;

    /* Compute the target number m = 6*n-3.  All operations use seq_t to
       keep the code generic for 8,16,32,64,128-bit or arbitrary-precision
       builds. */
    seq_t m = (seq_t)6 * n - 3;

    seq_t count = 0;
    seq_t d;

    /* Enumerate all divisors by checking up to sqrt(m).  Since m <= 60000
       for the intended usage, d*d will not overflow on any supported
       seq_t type. */
    for (d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            if (d % 6 == 5) ++count;
            seq_t q = m / d;
            if (q != d && q % 6 == 5) ++count;
        }
    }

    return count;
}
