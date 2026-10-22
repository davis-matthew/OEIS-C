/*
 * A368886  The largest unitary divisor of n without an exponent 2 in its
 *          prime factorization (A337050).
 * https://oeis.org/A368886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e==2 ? 1 : p^e) (highest unitary divisor of n
 *            without a p^2 factor)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based by OEIS convention; a(1)=1 */
    if (n == 0) return 0;                 /* unused, but harmless */

    seq_t result = 1;
    idx_t m = n;

    /* Trial division to factor n.  For each prime power p^e:  
       if e==2 the factor contributes 1, otherwise contribute p^e. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            if (e != 2)
            {
                seq_t pe = 1;
                for (idx_t i = 0; i < e; ++i)
                    pe *= (seq_t)p;
                result *= pe;
            }
        }
    }

    /* If n has a prime factor > sqrt(n), it appears once with e==1. */
    if (m > 1)
    {
        result *= (seq_t)m;
    }

    return result;
}
