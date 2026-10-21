/*
 * A290099  Multiplicative with a(2^e) = (-1)^e and a(p^e) = prevprime(p)^e
 *          for odd primes p.
 * https://oeis.org/A290099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = multiplicative with a(2^e)=(-1)^e, a(p^e)=prevprime(p)^e
 *            for odd primes p
 * Range:     i8 n <= 136, i16 n <= 8803, i32 n <= 8803, i64 n <= 8803,
 *            i128 n <= 8803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check if x is prime (trial division, suitable for n up to a few 10^5) */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Return the largest prime smaller than n (assumes n > 2) */
static uint64_t prev_prime(uint64_t n)
{
    for (uint64_t p = n - 1; p >= 2; --p)
        if (is_prime(p)) return p;
    return 0; /* should not happen for n > 2 */
}

/* Simple integer power: base^exp (exp is small for this sequence) */
static seq_t pow_uint(uint64_t base, uint64_t exp)
{
    seq_t res = 1;
    seq_t b = (seq_t)base;
    for (uint64_t i = 0; i < exp; ++i)
        res *= b;
    return res;
}

/* Recursive computation of the sequence */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Even n: a(n) = -a(n/2) */
    if ((n & 1) == 0)
        return -a(n >> 1);

    /* Odd n: factorise and multiply contributions */
    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;

    while (temp > 1)
    {
        /* Find smallest prime factor */
        uint64_t p = 0;
        if (temp % 2 == 0) p = 2;
        else
        {
            for (uint64_t i = 3; i * i <= temp; i += 2)
            {
                if (temp % i == 0)
                {
                    p = i;
                    break;
                }
            }
            if (p == 0) p = temp; /* temp is prime */
        }

        uint64_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        seq_t term;
        if (p == 2)
            term = (e & 1) ? (seq_t)-1 : (seq_t)1; /* unlikely for odd n */
        else
            term = pow_uint(prev_prime(p), e);

        prod *= term;
    }

    return prod;
}
