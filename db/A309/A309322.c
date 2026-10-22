/*
 * A309322  Expansion of Sum_{k>=1} phi(k) * x^k/(1 - x^k)^3, where phi =
 *          Euler totient function (A000010).
 * https://oeis.org/A309322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d * (d + 1)/2
 * Range:     u8 n <= 17, u16 n <= 263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses divisor enumeration; phi from utils. */

#include "../utils/phi.h"   /* provides seq_t phi(idx_t m); */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1; value undefined for 0 */
        return 0;

    seq_t result = 0;

    /* Enumerate divisors of n.  Use d * d <= n via d <= n/d to avoid overflow. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor d */
            idx_t q1 = n / d;                    /* n/d */
            seq_t phi_q1 = phi(q1);              /* phi(n/d) */
            seq_t term1 = phi_q1 * (seq_t)d * ((seq_t)d + 1) / 2;
            result += term1;

            /* second divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                idx_t q2 = n / d2;               /* n/(n/d) = d  */
                seq_t phi_q2 = phi(q2);          /* phi(n/(n/d)) = phi(d) */
                seq_t term2 = phi_q2 * (seq_t)d2 * ((seq_t)d2 + 1) / 2;
                result += term2;
            }
        }
    }

    return result;
}
