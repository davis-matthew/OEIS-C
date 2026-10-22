/*
 * A370625  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^3) )^n.
 * https://oeis.org/A370625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+k-1,k) *
 *            binomial(n-2*k-1,n-3*k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t total = 0;
    idx_t limit = n / 3;

    for (idx_t k = 0; k <= limit; ++k)
    {
        /* For the second binomial we need n >= 2*k+1 to avoid underflow */
        if (n <= 2 * k)
            continue;

        /* The first binomial is always defined for current k */
        seq_t t1 = binomial(n + k - 1, k);

        /* Compute arguments for the second binomial */
        idx_t m1 = n - 2 * k - 1;   /* upper index */
        idx_t m2 = n - 3 * k;       /* lower index */

        /* Skip if lower index exceeds upper index */
        if (m2 > m1)
            continue;

        seq_t t2 = binomial(m1, m2);

        total += t1 * t2;
    }

    return total;
}
