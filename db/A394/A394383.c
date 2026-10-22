/*
 * A394383  a(n) = f^(n/f), where f is the largest prime factor of n.
 * https://oeis.org/A394383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A006530(n)^(n/A006530(n)) (largest prime factor to the
 *            power of n / that factor)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 242, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* a(1) = 1; the definition is only valid for n >= 1 */
    if (n <= 1u)
        return 1u;

    /* Find the largest prime factor of n by trial division. */
    uint64_t m = (uint64_t)n;          /* copy of n for manipulation */
    uint64_t largest = 1u;

    for (uint64_t p = 2u; p * p <= m; ++p)
    {
        while (m % p == 0u)
        {
            largest = p;
            m /= p;
        }
    }
    if (m > 1u)
        largest = m;                    /* m itself is a prime > sqrt(original n) */

    /* exponent is n / largest */
    idx_t exp = n / (idx_t)largest;

    /* compute largest^exp using the provided arbitrary‑precision power routine */
    return pow_uint(largest, exp);
}
