/*
 * A380444  Sum of the nonprimes dividing n and the squares of the primes
 *          dividing n.
 * https://oeis.org/A380444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - S(p|n) + Σ_{p|n} p^2 * where S(p|n) is the sum
 *            of the distinct prime divisors of n, * sigma(n) is the sum of
 *            all divisors of n. * This implements the definition of OEIS
 *            A380444
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t nn)
{
    /* Work in native 64‑bit for the factorisation */
    uint64_t n = (uint64_t)nn;
    if (n == 0) return (seq_t)0;     /* Sequence is defined for n>=1, but guard */
    uint64_t temp = n;

    seq_t sigma = (seq_t)1;          /* product over prime power sums */
    seq_t sopf = (seq_t)0;           /* distinct prime sum */
    seq_t prime_squares = (seq_t)0;  /* sum of squares of distinct primes */

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            seq_t sigma_factor = (seq_t)1;
            seq_t p_pow = (seq_t)1;

            /* Count exponent and all needed multiplicity for sigma */
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
                p_pow *= (seq_t)p;
                sigma_factor += p_pow;
            }

            sigma *= sigma_factor;           /* multiply contribution of this prime power */
            sopf   += (seq_t)p;              /* distinct prime once */
            prime_squares += (seq_t)p * (seq_t)p;
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        seq_t p = (seq_t)temp;
        /* Exponent e=1 */
        sigma *= ((seq_t)1 + p);
        sopf   += p;
        prime_squares += p * p;
    }

    return sigma - sopf + prime_squares;
}
