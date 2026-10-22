/*
 * A372963  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4 <= n} ( n/gcd(x_1, x_2, x_3,
 *          x_4, n) )^2.
 * https://oeis.org/A372963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * (n/d)^2 * sigma_6(d) Multiplicative
 *            evaluation: a(p^e) = (p^(6e+6) - p^(6e+2) + p^2 - 1) / (p^6 - 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   pow128_uint64, term_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For e = 1 the above simplifies to a(p) = p^6 - p^2 + 1, avoiding
   the need for p^12 and keeping all intermediate values within
   128‑bit arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow128_uint64.h"
#include "../utils/term_prime_power.h"

/* Main function for the OEIS sequence A372963 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1          */
    seq_t result = 1;
    idx_t m = n;                         /* factorise n                   */

    /* trial division – works for n up to at least 10000       */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= term_prime_power(p, e);
        }
    }

    /* any remaining prime factor > sqrt(original n) has e==1   */
    if (m > 1)
        result *= term_prime_power(m, 1);

    return result;
}
