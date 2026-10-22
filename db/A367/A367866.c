/*
 * A367866  a(n) = Sum_{d|n} d * sigma(d) * mu(d)^2.
 * https://oeis.org/A367866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma(rad(n)^2)
 * Range:     u8 n <= 13, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 1.. */
    seq_t result = 1;
    idx_t m = n;
    idx_t p = 2;

    /* trial division to find distinct prime factors of n */
    while (p <= m / p)
    {
        if (m % p == 0)
        {
            /* include the prime factor once */
            seq_t pp = (seq_t)p * (seq_t)p;
            result *= (seq_t)1 + (seq_t)p + pp;

            /* remove all powers of p from m */
            do {
                m /= p;
            } while (m % p == 0);
        }
        /* after 2, test only odd numbers */
        p = (p == 2) ? 3 : p + 2;
    }

    /* if a prime factor > sqrt remains */
    if (m > 1)
    {
        seq_t mm = (seq_t)m * (seq_t)m;
        result *= (seq_t)1 + (seq_t)m + mm;
    }

    return result;
}
