/*
 * A396515  a(n) = Sum_{k=1..n} gcd(k,n)^(ceiling(n/k) - floor(n/k)).
 * https://oeis.org/A396515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} ( d*phi(n/d)-d+1 )
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* Provides seq_t phi(idx_t m) */

/* Computes the OEIS sequence A396515.
   Input:  n >= 1
   Output: a(n) as defined above. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d1 = i, its complement d2 = n / i */
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* contribution for d1 */
            seq_t phi1 = phi(d2);                     /* phi(n/d1) */
            seq_t contrib1 = (seq_t)d1 * phi1 - (seq_t)d1 + (seq_t)1;
            total += contrib1;

            /* if d1 and d2 differ, add the second divisor */
            if (d1 != d2)
            {
                seq_t phi2 = phi(d1);                 /* phi(n/d2) */
                seq_t contrib2 = (seq_t)d2 * phi2 - (seq_t)d2 + (seq_t)1;
                total += contrib2;
            }
        }
    }

    return total;
}
