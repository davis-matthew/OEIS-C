/*
 * A348028  Greatest common divisor of A003415 (arithmetic derivative) and
 *          sigma, the sum of divisors function.
 * https://oeis.org/A348028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(σ(n), A003415(n))
 * Range:     u8 n <= 691, u16 n <= 9483, u32 n <= 9483, u64 n <= 9483,
 *            u128 n <= 9483, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but safe */

    idx_t temp   = n;
    seq_t sigma  = 1;   /* σ(1) = 1 */
    seq_t D      = 0;   /* arithmetic derivative */
    seq_t n_seq  = (seq_t)n;

    /* collect primes and their exponents (max 64 distinct primes for uint64_t) */
    idx_t primes[64];
    idx_t exps  [64];
    size_t cnt  = 0;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1)
    {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Compute σ(n) and the arithmetic derivative */
    for (size_t i = 0; i < cnt; ++i)
    {
        idx_t p = primes[i];
        idx_t e = exps[i];

        /* σ contribution: 1 + p + p² + … + p^e */
        seq_t term = 1;
        seq_t pow  = 1;
        for (idx_t k = 1; k <= e; ++k)
        {
            pow  *= (seq_t)p;
            term += pow;
        }
        sigma *= term;

        /* derivative contribution: e * (n / p) */
        seq_t partial = n_seq / (seq_t)p;  /* exact division */
        partial *= (seq_t)e;
        D += partial;
    }

    return gcd(sigma, D);
}
