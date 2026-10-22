/*
 * A319089  a(n) = tau(n)^3, where tau is A000005.
 * https://oeis.org/A319089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = tau(n)^3 where tau(n) is the number of divisors of n
 *            (A000005). The computation uses a trial‑division factorisation
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only what is necessary for fixed‑width integer types. */
#include <stdint.h>

/* Use the types supplied by the build system. */
seq_t a(idx_t n)
{
    /* OEIS defines the offset as 1; values for n < 1 are undefined. */
    if (n < 1) return 0;

    /* Factorisation of n.  Use the natural type idx_t for the current value. */
    idx_t m = n;
    seq_t tau = 1;                /* divisor count accumulator */

    /* Trial division up to sqrt(m).  The condition m/p works without
       overflow because p > 0 and m/p decreases as p increases. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            /* Count exponent of prime p. */
            idx_t e = 0;
            do
            {
                m /= p;
                ++e;
            } while (m % p == 0);

            /* Update tau: multiply by (e + 1).  The cast ensures
               the multiplication is performed in the type seq_t. */
            tau *= (seq_t)(e + 1);
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (m > 1) tau *= (seq_t)2;

    /* Compute tau^3.  Multiplying three times is fine; intermediate
       overflow is acceptable because the result is taken modulo the
       width of seq_t, exactly as required by the problem statement. */
    seq_t res = tau * tau * tau;
    return res;
}
