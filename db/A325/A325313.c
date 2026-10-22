/*
 * A325313  a(n) = A048250(n) - n, where A048250(n) is the sum of squarefree
 *          divisors of n.
 * https://oeis.org/A325313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{p|n, p prime} (1 + p) – n (sum of squarefree divisors
 *            of n minus n)
 * Range:     i8 n <= 137, i16 n <= 8989, i32 n <= 8989, i64 n <= 8989,
 *            i128 n <= 8989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* for safety – offset is 1 */
    seq_t prod = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)           /* distinct prime factor found */
        {
            prod *= (seq_t)(p + 1);
            while (temp % p == 0)    /* remove all copies of p */
                temp /= p;
        }
    }
    if (temp > 1)                    /* remaining prime > sqrt(original n) */
        prod *= (seq_t)(temp + 1);

    return prod - (seq_t)n;
}
