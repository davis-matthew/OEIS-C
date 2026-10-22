/*
 * A328945  Numbers m that are neither arithmetic (A003601) nor harmonic
 *          (A001599).
 * https://oeis.org/A328945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328945: integers m such that neither the arithmetic mean
 * A(m)=σ(m)/d(m) nor the harmonic mean
 * H(m)=m·d(m)/σ(m) of the divisors of m is an integer.
 * No closed formula is known; the routine scans m=2,3,… and tests
 * both divisibility conditions by simple trial factoring.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether m satisfies the “neither A(m) nor H(m) is integer” condition.
 * The function factors m by trial division and computes σ(m) and d(m) on the fly.
 */
static bool good_m(uint64_t m)
{
    seq_t sigma = 1;          /* σ(m) – sum of all divisors */
    seq_t divisors = 1;       /* d(m) – number of divisors */

    uint64_t orig = m;        /* preserve original value for the sqrt bound */
    uint64_t tmp  = m;        /* mutable copy for dividing out prime powers */

    for (uint64_t p = 2; p * p <= orig; ++p)
    {
        if (tmp % p == 0)
        {
            /* Count exponent e of prime p. */
            uint32_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            /* Compute σ(p^e) = 1 + p + p^2 + … + p^e. */
            seq_t pe   = 1;   /* p^0 */
            seq_t sum  = 1;   /* sum of powers, starts with 1 */

            for (uint32_t k = 1; k <= e; ++k)
            {
                pe  *= (seq_t)p;
                sum += pe;
            }

            sigma     *= sum;
            divisors  *= (seq_t)(e + 1);
        }
    }

    /* Remaining factor (if any) is a prime > sqrt(orig). */
    if (tmp > 1)
    {
        seq_t sum = 1 + (seq_t)tmp;
        sigma    *= sum;
        divisors *= 2;
    }

    /* Arithmetic mean integer iff σ(m) % d(m) == 0. */
    if (sigma % divisors == 0)
        return false;

    /* Harmonic mean integer iff (m * d(m)) % σ(m) == 0. */
    seq_t prod = (seq_t)orig * divisors;
    if (prod % sigma == 0)
        return false;

    return true;
}

/* External interface required by the harness. */
seq_t a(idx_t n)
{
    /* n >= 1; the sequence starts at a(1)=2. */
    uint64_t count = 0;
    uint64_t m     = 1;       /* start scanning from 2 */

    while (true)
    {
        ++m;
        if (good_m(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
