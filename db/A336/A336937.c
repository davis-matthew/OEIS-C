/*
 * A336937  The 2-adic valuation of sigma(n), the sum of divisors of n.
 * https://oeis.org/A336937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 2 in sigma(n) (the sum of divisors of n)
 * Range:     u8 n <= 8387, u16 n <= 8387, u32 n <= 8387, u64 n <= 8387,
 *            u128 n <= 8387, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);   /* compute σ(n) */
    seq_t v2 = 0;                /* 2‑adic valuation of σ(n) */
    while ((sigma & 1) == 0)
    {
        sigma >>= 1;
        v2++;
    }
    return v2;
}
