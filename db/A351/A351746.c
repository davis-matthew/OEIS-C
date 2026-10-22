/*
 * A351746  a(n) = Sum_{p|n, p prime} (p-1) * tau(n/p).
 * https://oeis.org/A351746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} (p-1) * tau(n/p) (tau = divisor count)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)                 /* a(1) = 0, and the formula has no terms for n = 1 */
        return 0;

    uint64_t m = (uint64_t)n;   /* factor n using trial division */

    /* at most log_2(n) distinct primes, ≤ 64 for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t nf = 0;               /* number of distinct prime factors */

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            primes[nf] = p;
            exps[nf]   = e;
            ++nf;
        }
    }
    if (m > 1)                   /* remaining prime factor */
    {
        primes[nf] = m;
        exps[nf]   = 1;
        ++nf;
    }

    /* compute tau(n) = product (exponent_i + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < nf; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* compute the required sum */
    seq_t sum = 0;
    for (size_t i = 0; i < nf; ++i)
    {
        seq_t tau_np = (tau * (seq_t)exps[i]) / (seq_t)(exps[i] + 1);
        sum += (seq_t)(primes[i] - 1) * tau_np;
    }

    return sum;
}
