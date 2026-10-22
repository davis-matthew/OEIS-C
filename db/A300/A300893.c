/*
 * A300893  L.g.f.: log(Product_{k>=1} (1 + x^k)/(1 + x^prime(k))) =
 *          Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A300893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = Σ_{d|n} (-1)^(d+1) * (n/d) - Σ_{p|n prime} (-1)^(n/p +
 *            1) * p * * a(n) can be computed with O(√n) divisor enumeration
 *            and O(√n) prime factorization. * no dynamic allocation needed
 * Range:     i8 n <= 98, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nn = n;                     /* store original n */
    seq_t sum1 = 0;                   /* Σ over all divisors */
    seq_t sum2 = 0;                   /* Σ over prime divisors */

    /* Sum over all divisors of n */
    for (idx_t d = 1; d <= nn / d; ++d)
    {
        if (nn % d == 0)
        {
            /* divisor d */
            seq_t term = (seq_t)(nn / d);
            if (d & 1)   /* d odd → (+1) */
                sum1 += term;
            else         /* d even → (-1) */
                sum1 -= term;

            /* complementary divisor e = n/d */
            idx_t e = nn / d;
            if (e != d)
            {
                term = (seq_t)(nn / e);
                if (e & 1)   /* e odd → (+1) */
                    sum1 += term;
                else         /* e even → (-1) */
                    sum1 -= term;
            }
        }
    }

    /* Prime factorization to collect distinct prime divisors */
    idx_t temp = nn;
    idx_t primes[64];
    idx_t pcnt   = 0;
    idx_t p      = 2;
    while (p <= temp / p)
    {
        if (temp % p == 0)
        {
            primes[pcnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
        ++p;
    }
    if (temp > 1)
        primes[pcnt++] = temp;

    /* Sum over prime divisors */
    for (idx_t i = 0; i < pcnt; ++i)
    {
        idx_t prime = primes[i];
        seq_t term  = (seq_t)(nn / prime);           /* n / p  */
        /* sign = (-1)^(term + 1) */
        if (((term + 1) & 1) == 0)   /* term+1 even → (+1) */
            sum2 += (seq_t)prime;
        else                        /* term+1 odd → (-1) */
            sum2 -= (seq_t)prime;
    }

    return sum1 - sum2;
}
