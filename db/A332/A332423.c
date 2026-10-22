/*
 * A332423  If n = Product (p_j^k_j) then a(n) = Sum ((-1)^(k_j + 1) * k_j).
 * https://oeis.org/A332423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{prime factor p^k | n} (-1)^(k+1) * k (sum of odd
 *            exponents minus sum of even exponents in the prime
 *            factorization)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n <= 1)
        return res;                       /* a(1) = 0, a(0) not used */

    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) /* trial division up to sqrt(temp) */
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            res += (e & 1) ? (seq_t)e : -(seq_t)e;
        }
    }

    /* If temp > 1, the remaining factor is prime with exponent 1 */
    if (temp > 1)
        res += 1;                         /* exponent 1 is odd */

    return res;
}
