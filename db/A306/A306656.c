/*
 * A306656  Number of ways to fill a 3D matrix with n distinct values.
 * https://oeis.org/A306656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007425(n) * n! (n > 0, a(0) = 1) A007425(n) = ∏_{p^e‖n}
 *            (e+2)(e+1)/2 where the product runs over the prime factorisation
 *            of n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    /* A007425(n) */
    seq_t prod = 1;
    idx_t m = n;
    for (idx_t p = 2U; p * p <= m; ++p)
    {
        if (m % p == 0U)
        {
            idx_t e = 0;
            while (m % p == 0U)
            {
                m /= p;
                ++e;
            }
            seq_t term = ((seq_t)(e + 2) * (seq_t)(e + 1)) / 2;
            prod *= term;
        }
    }
    if (m > 1U)          /* remaining prime factor with exponent 1 */
        prod *= (seq_t)3;

    /* n! */
    seq_t fact = 1;
    for (idx_t i = 2U; i <= n; ++i)
        fact *= (seq_t)i;

    return prod * fact;
}
