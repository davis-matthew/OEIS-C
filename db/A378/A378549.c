/*
 * A378549  Sum of divisors d of n such that n/d is not an odd number with an
 *          even number of prime factors (counted with multiplicity).
 * https://oeis.org/A378549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, [not( (n/d is odd) && (omega(n/d) even) )]} d
 * Range:     u8 n <= 167, u16 n <= 8364, u32 n <= 8364, u64 n <= 8364,
 *            u128 n <= 8364, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety guard for illegal argument */
    seq_t total = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n / i; ++i)
    {
        if (n % i == (idx_t)0)
        {
            idx_t d1 = i;          /* divisor */
            idx_t d2 = n / i;      /* complementary divisor = n / d1 */

            /* first divisor d1, q = d2 */
            if (!((d2 & (idx_t)1) && ((bigomega(d2) & (seq_t)1) == (seq_t)0)))
                total += (seq_t)d1;

            /* second divisor d2, ensure distinct from d1 */
            if (d1 != d2)
            {
                if (!((d1 & (idx_t)1) && ((bigomega(d1) & (seq_t)1) == (seq_t)0)))
                    total += (seq_t)d2;
            }
        }
    }
    return total;
}
