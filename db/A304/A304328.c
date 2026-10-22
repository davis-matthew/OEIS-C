/*
 * A304328  a(n) = n/(largest perfect power divisor of n).
 * https://oeis.org/A304328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / (largest perfect power divisor of n)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Small helper for integer powers: base^exp (exp >= 0) */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

/* Returns the largest perfect power divisor of n. */
static seq_t largest_perfect_power_divisor(idx_t n)
{
    /* Factorise n by trial division. */
    uint64_t primes[20];
    int      exps[20];
    size_t   pcnt = 0;
    idx_t    remaining = n;
    int      max_exp = 0;

    idx_t p = 2;
    while ((idx_t)(p * p) <= remaining)
    {
        if (remaining % p == 0)
        {
            int e = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++e;
            }
            primes[pcnt]   = (uint64_t)p;
            exps[pcnt]     = e;
            if (e > max_exp)
                max_exp = e;
            ++pcnt;
        }
        ++p;
    }
    if (remaining > 1)
    {
        primes[pcnt]   = remaining;
        exps[pcnt]     = 1;
        if (1 > max_exp)
            max_exp = 1;
        ++pcnt;
    }

    /* No perfect power divisor > 1? */
    if (max_exp <= 1)
        return (seq_t)1;

    seq_t best = 1;
    for (int k = 2; k <= max_exp; ++k)
    {
        seq_t d = 1;
        for (size_t i = 0; i < pcnt; ++i)
        {
            int pk = (exps[i] / k) * k;
            if (pk > 0)
                d *= pow_uint(primes[i], pk);
        }
        if (d > best)
            best = d;
    }
    return best;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t lp = largest_perfect_power_divisor(n);
    return (seq_t)n / lp;
}
