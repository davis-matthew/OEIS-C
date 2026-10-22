/*
 * A328613  Starting with m = 1, p = 2 and x = n, iterate m -> m *
 *          p^valuation(x,p), x -> floor(x/p), p -> {the next prime after p},
 *          until x is zero. Then a(n) = product of the values of m and p that
 *          were reached at the end of the iteration.
 * https://oeis.org/A328613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the final m and p values after the iterative
 *            valuation process described for OEIS sequence A328613
 * Range:     u8 n <= 29, u16 n <= 863, u32 n <= 8502, u64 n <= 8502,
 *            u128 n <= 8502, big any n
 * Helpers:   next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    idx_t x  = n;          /* current value in the loop */
    uint64_t p = 2;        /* current prime */
    seq_t m = 1;           /* accumulated product */

    while (x > 0)
    {
        /* valuation of x at prime p */
        idx_t temp = x;
        idx_t val = 0;
        while (temp % p == 0)
        {
            temp /= p;
            val++;
        }

        if (val > 0)
        {
            m *= pow_uint(p, val);   /* multiply by p^val */
        }

        x /= p;           /* floor division by p */
        p = next_prime(p); /* advance to next prime */
    }

    return m * (seq_t)p;   /* product of final m and p */
}
