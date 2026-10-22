/*
 * A307430  Dirichlet g.f.: zeta(s) / zeta(4*s).
 * https://oeis.org/A307430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is 4th‑power‑free (no prime divisor with exponent
 *            ≥4), 0 otherwise. Runtime: O(√n) by trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* offset is 1, but safe for n==0 */
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            do
            {
                exp++;
                m /= p;
                if (exp >= 4) return (seq_t)0;   /* found a prime^4 divisor */
            } while (m % p == 0);
        }
    }
    /* Remaining factor (if any) has exponent 1, hence acceptable. */
    return (seq_t)1;
}
