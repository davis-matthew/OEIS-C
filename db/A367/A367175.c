/*
 * A367175  a(n) = Sum_{d|n} (-1)^[d is prime] * d, where [] denotes the
 *          Iverson bracket.
 * https://oeis.org/A367175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^[d is prime] * d
 * Range:     i8 n <= 59, i16 n <= 9239, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  OEIS offset: a(1) is the first term.  */
    if (n == 0)
        return 0;              /* outside the sequence; harmless */

    uint64_t temp = (uint64_t)n;
    seq_t sigma = 1;           /* product of sigma(p^e) for each prime factor */
    seq_t sum_primes = 0;      /* sum of distinct prime divisors */

    uint64_t p = 2;
    while (p <= temp / p)     /* use division to avoid overflow */
    {
        if (temp % p == 0)
        {
            /* factor p^exp */
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            seq_t pseq = (seq_t)p;
            seq_t sigma_part = 1;   /* 1 */
            seq_t pow = 1;
            for (int i = 0; i < exp; ++i)
            {
                pow = pow * pseq;          /* p^i */
                sigma_part = sigma_part + pow;  /* add to sigma part */
            }
            sigma = sigma * sigma_part;
            sum_primes = sum_primes + pseq;
        }
        ++p;
    }

    /* Remaining factor > 1, if any */
    if (temp > 1)
    {
        seq_t pseq = (seq_t)temp;
        seq_t sigma_part = 1 + pseq;     /* sigma(p) = 1 + p */
        sigma = sigma * sigma_part;
        sum_primes = sum_primes + pseq;
    }

    seq_t two = (seq_t)2;
    return sigma - (two * sum_primes);
}
