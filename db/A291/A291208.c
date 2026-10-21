/*
 * A291208  Number of noncube divisors of n.
 * https://oeis.org/A291208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n minus number of divisors of n
 *            that are perfect cubes. Equivalent to a(n) = d(n) – Π_i
 *            (⌊e_i/3⌋+1), where e_i are the exponents in the prime
 *            factorization of n
 * Range:     u8 n <= 9696, u16 n <= 9696, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* factor_n divides n into arrays pr[] and exp[].
   The caller must provide arrays large enough (e.g. size 64). */
static void factor_n(uint64_t n, uint64_t pr[], uint64_t exp[], size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[(*cnt)]   = p;
            exp[(*cnt)]  = e;
            ++(*cnt);
        }
    }
    if (n > 1)            /* remaining prime factor */
    {
        pr[(*cnt)]  = n;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    /* Handle n == 0 by returning 0 (outside the offset, behavior undefined). */
    if (n == 0)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_n((uint64_t)n, primes, exps, &cnt);

    seq_t total = (seq_t)1;
    seq_t cube  = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        total *= (seq_t)(exps[i] + 1);
        cube  *= (seq_t)(exps[i] / 3 + 1);
    }
    /* For n == 1, cnt == 0, total == 1, cube == 1, result == 0. */
    return total - cube;
}
