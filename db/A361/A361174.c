/*
 * A361174  The sum of the exponential squarefree exponential divisors (or
 *          e-squarefree e-divisors) of n.
 * https://oeis.org/A361174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e || n} sum_{d|e, d squarefree} p^d
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Helper: factor_u64_local
 *  factorises n into prime factors stored in pr[] and ex[].
 *  pr, ex must have space for at least 64 entries.
 *  stores the count in *cnt.
 */
static void factor_u64_local(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    uint64_t temp = n;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            size_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            pr[i] = p;
            ex[i] = exp;
            ++i;
        }
    }
    if (temp > 1)   /* remaining prime factor */
    {
        pr[i] = temp;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* ------------------------------------------------------------ */
/* Main function implementing the sequence A361174 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, but safe guard */
    if (n == 1)
        return 1;           /* a(1) = 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64_local((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t local = 0;
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* sum_{d|e, d squarefree} p^d */
        for (uint64_t d = 1; d <= e; ++d)
        {
            if (e % d != 0)
                continue;

            /* test squarefree: check for any k^2 | d */
            bool sqf = true;
            for (uint64_t k = 2; k * k <= d; ++k)
                if (d % (k * k) == 0)
                {
                    sqf = false;
                    break;
                }
            if (!sqf)
                continue;

            /* compute p^d */
            seq_t power = 1;
            for (uint64_t j = 0; j < d; ++j)
                power *= (seq_t)p;

            local += power;
        }

        result *= local;
    }

    return result;
}
