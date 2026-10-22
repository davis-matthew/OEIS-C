/*
 * A385134  The sum of divisors d of n such that n/d is a biquadratefree
 *          number (A046100).
 * https://oeis.org/A385134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} d if n/d is 4th power free (biquadratefree)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   term_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative with a(p^e) = p^max(e-3,0) * (p^min(e+1,4)-1)/(p-1) */
#include <stdint.h>
#include "../utils/term_for_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* sequence is undefined for n<1 */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* factor 2 separately */
    if ((temp & 1) == 0)
    {
        uint64_t e = 0;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            ++e;
        }
        result *= term_for_prime((seq_t)2, e);
    }

    /* factor odd primes */
    for (uint64_t i = 3; i <= temp / i; i += 2)
    {
        if (temp % i == 0)
        {
            uint64_t e = 0;
            while (temp % i == 0)
            {
                temp /= i;
                ++e;
            }
            result *= term_for_prime((seq_t)i, e);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* exponent is 1 */
        result *= term_for_prime((seq_t)temp, 1);
    }

    return result;
}
