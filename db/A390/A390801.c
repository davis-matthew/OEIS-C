/*
 * A390801  The sum of the integers k from 1 to n such that gcd(n, k) is
 *          prime.
 * https://oeis.org/A390801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p * f(n/p), where f(N) = N * phi(N) /
 *            2 (N > 1) and f(1) = 1. This is equivalent to the definition:
 *            sum of k (1 ≤ k ≤ n) such that gcd(n, k) is prime
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute Euler's totient function φ(m) as seq_t.
   m is never larger than 65536 in the expected use. */
static seq_t _phi_u64(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            result -= result / p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0, a(0) unused */
        return 0;

    seq_t sum = 0;
    idx_t temp = n;

    /* iterate over prime divisors p of n */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* p is a prime divisor */
            idx_t m = n / p;           /* N = n/p */
            seq_t f;
            if (m == 1)
                f = 1;
            else
            {
                seq_t phi_m = _phi_u64(m);
                f = (seq_t)m * phi_m / 2;
            }
            sum += (seq_t)p * f;

            /* remove p from temp so we don't see it again */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        idx_t p = temp;
        idx_t m = n / p;
        seq_t f;
        if (m == 1)
            f = 1;
        else
        {
            seq_t phi_m = _phi_u64(m);
            f = (seq_t)m * phi_m / 2;
        }
        sum += (seq_t)p * f;
    }

    return sum;
}
