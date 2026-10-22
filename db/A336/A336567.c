/*
 * A336567  Sum of proper divisors of {n divided by its largest squarefree
 *          divisor}.
 * https://oeis.org/A336567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of proper divisors of n / rad(n)
 * Range:     u8 n <= 863, u16 n <= 9729, u32 n <= 9729, u64 n <= 9729,
 *            u128 n <= 9729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = σ(n / rad(n)) – (n / rad(n))               */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) – cast to a 64–bit integer for internal use */
    uint64_t nn   = (uint64_t)n;
    uint64_t temp = nn;               /* temporary value for factorisation */

    /* sigma(m) where m = n / rad(n)  (product of p^(e–1))   */
    seq_t sigma = 1;
    seq_t m     = 1;

    /* factorise n; for each prime p^e that divides n            */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            /* compute p^exp as seq_t */
            seq_t power = 1;
            for (uint64_t i = 0; i < exp; ++i)
                power *= (seq_t)p;

            /* sigma contribution : (p^exp – 1)/(p – 1)              */
            seq_t term = (power - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= term;

            /* contribution to m : p^(exp – 1)                       */
            if (exp > 1)
            {
                seq_t mpow = 1;
                for (uint64_t i = 0; i < exp - 1; ++i)
                    mpow *= (seq_t)p;
                m *= mpow;
            }
        }
    }

    /* any remaining factor > 1 is a prime with exponent 1
       – its contribution to σ(m) is 1 and to m is 1, so nothing to do   */
    if (temp > 1)
    {
        /* nothing: exponent = 1 ⇒ p^(exp–1) = 1 and (p^exp–1)/(p–1) = 1 */
    }

    /* a(n) = σ(m) – m, with m = n / rad(n)                          */
    return sigma - m;
}
