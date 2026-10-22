/*
 * A345371  Number of squarefree divisors of n whose square does not divide n.
 * https://oeis.org/A345371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^k - 2^t where k = the number of distinct prime factors
 *            of n t = the number of distinct prime factors of n having
 *            exponent ≥ 2 This implements the OEIS sequence A345371: the
 *            number of squarefree divisors of n whose square does not divide
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_of_two_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/power_of_two_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, but return 0 */

    uint64_t m = (uint64_t)n;
    size_t k = 0;                 /* distinct prime factors count */
    size_t t = 0;                 /* distinct prime factors with exponent ≥2 */

    /* Trial division factorisation */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            ++k;
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if (exp >= 2) ++t;
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1)
    {
        ++k;
        /* exponent is 1 ⇒ does not increment t */
    }

    /* a(n) = 2^k - 2^t */
    return power_of_two_seq(k) - power_of_two_seq(t);
}
