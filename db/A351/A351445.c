/*
 * A351445  a(n) = A003958(sigma(n)) - A003958(n), where A003958 is
 *          multiplicative with a(p^e) = (p-1)^e and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A351445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003958(sigma(n)) - A003958(n) where A003958 is
 *            multiplicative with a(p^e) = (p-1)^e and sigma is the
 *            sum-of-divisors function
 * Range:     i8 n <= 48, i16 n <= 8487, i32 n <= 8487, i64 n <= 8487,
 *            i128 n <= 8487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* harmless for offset 1 */
    uint64_t x = (uint64_t)n;

    /* factor n, compute sigma(n) and f(n) */
    seq_t sigma = 1;                         /* sigma(n) */
    seq_t fn = 1;                            /* A003958(n) */

    /* factor 2 */
    uint64_t tmp = x;
    int e = 0;
    while ((tmp & 1ULL) == 0)
    {
        tmp >>= 1;
        ++e;
    }
    if (e > 0)
    {
        /* (2-1)^e == 1, so fn unchanged */
        /* sigma_2 = 2^(e+1) - 1 */
        sigma *= (pow_seq(2, e + 1) - 1);
    }

    /* factor odd primes */
    for (uint64_t d = 3; d * d <= tmp; d += 2)
    {
        if (tmp % d == 0)
        {
            e = 0;
            while (tmp % d == 0)
            {
                tmp /= d;
                ++e;
            }
            fn *= pow_seq((seq_t)(d - 1), e);
            /* sigma_d = (d^(e+1)-1)/(d-1) */
            sigma *= (pow_seq((seq_t)d, e + 1) - 1) / (seq_t)(d - 1);
        }
    }
    if (tmp > 1)                /* remaining prime */
    {
        fn *= (seq_t)(tmp - 1);
        sigma *= (seq_t)(tmp + 1);   /* since sigma(tmp) = tmp + 1 */
    }

    /* factor sigma(n) and compute f(sigma(n)) */
    seq_t f_sigma = 1;
    uint64_t s = (uint64_t)sigma;    /* sigma fits into uint64_t for the dataset */

    /* factor 2 in sigma */
    e = 0;
    while ((s & 1ULL) == 0)
    {
        s >>= 1;
        ++e;
    }
    if (e > 0)
    {
        /* (2-1)^e == 1, so f_sigma unchanged */
    }

    /* factor odd primes in sigma */
    for (uint64_t d = 3; d * d <= s; d += 2)
    {
        if (s % d == 0)
        {
            e = 0;
            while (s % d == 0)
            {
                s /= d;
                ++e;
            }
            f_sigma *= pow_seq((seq_t)(d - 1), e);
        }
    }
    if (s > 1)                     /* remaining prime in sigma */
    {
        f_sigma *= (seq_t)(s - 1);
    }

    return f_sigma - fn;
}
