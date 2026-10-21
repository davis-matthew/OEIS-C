/*
 * A255369  a(n) = (sigma(n)-n-1)*(2-mu(n)), where sigma(n) is the sum of the
 *          divisors of n and mu(n) is the Möbius function.
 * https://oeis.org/A255369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sigma(n)-n-1)*(2-mu(n))
 * Range:     i8 n <= 41, i16 n <= 6089, i32 n <= 9293, i64 n <= 9293,
 *            i128 n <= 9293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute sigma(n) as sum of divisors */
    seq_t sigma = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            sigma += (seq_t)d;
            idx_t other = n / d;
            if (other != d)
                sigma += (seq_t)other;
        }
    }

    /* compute mu(n) (Mobius function) */
    int mu = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
                if (exp > 1) { mu = 0; break; }   /* square factor found */
            }
            if (mu == 0) break;
            mu = -mu;  /* flip sign for each distinct prime factor */
        }
    }
    if (mu != 0 && temp > 1)   /* remaining prime factor */
        mu = -mu;

    /* final computation */
    seq_t part1 = sigma - (seq_t)n - 1;
    seq_t part2 = 2 - (seq_t)mu;
    return part1 * part2;
}
