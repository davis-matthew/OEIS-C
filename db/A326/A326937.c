/*
 * A326937  Dirichlet g.f.: (2^s - 1) / (zeta(s-1) * (2^s - 2)).
 * https://oeis.org/A326937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326937
 * Formula: a(n) = 0 if n is not squarefree;
 *            otherwise a(n) = (-1)^{ω(n)} * n / 2^{[2|n]},
 *            where ω(n) is the number of distinct prime factors of n.
 * Recurrence (used here for reference):
 *    a(1) = 1;
 *    a(n) = -∑_{d|n, d<n} A000265(n/d) * a(d).
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t remaining = (uint64_t)n;
    idx_t prime_count = 0;
    seq_t prod = 1;
    bool square_factor = false;

    /* trial division up to sqrt(n) */
    for (idx_t p = 2; p <= n / p; ++p)
    {
        if (remaining % p == 0)
        {
            ++prime_count;
            uint64_t exp = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++exp;
                if (exp > 1)            /* found a squared prime factor */
                {
                    square_factor = true;
                    break;
                }
            }
            if (square_factor)
                break;
            if (p != 2)                /* 2 contributes |factor| = 1 */
                prod *= (seq_t)p;
        }
    }

    if (!square_factor && remaining > 1)
    {
        /* remaining is a prime > sqrt(n) */
        ++prime_count;
        if (remaining != 2)
            prod *= (seq_t)remaining;
    }

    if (square_factor)
        return 0;

    seq_t sign = (prime_count % 2 == 0) ? 1 : -1;
    return sign * prod;
}
