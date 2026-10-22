/*
 * A326005  G.f.: Sum_{n>=0} (n+1)*(n+2)*(n+3)*(n+4)/4! * x^n * (1 + x^n)^n.
 * https://oeis.org/A326005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=∑_{k|n, 1≤n/k≤k+1} binomial(k+4,4) *
 *            binomial(k,n/k-1)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the binomial helper provided by the framework */
#include "../utils/binomial.h"

/* Computes A326005(a) for any n>=0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = (seq_t)0;         /* running total */

    /* Enumerate all positive divisors k of n. */
    for (idx_t d = (idx_t)1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;              /* d is not a divisor */

        /* First divisor k = d */
        idx_t k = d;
        idx_t m = n / k;           /* m = n / k */
        if (m <= k + (idx_t)1)
        {
            seq_t t1 = binomial(k + (idx_t)4, (idx_t)4);
            seq_t t2 = binomial(k, m - (idx_t)1);
            sum += t1 * t2;
        }

        /* Paired divisor k = n / d, distinct from d */
        idx_t k2 = n / d;
        if (k2 == k)                 /* already processed */
            continue;

        idx_t m2 = n / k2;           /* m2 = d */
        if (m2 <= k2 + (idx_t)1)
        {
            seq_t t1 = binomial(k2 + (idx_t)4, (idx_t)4);
            seq_t t2 = binomial(k2, m2 - (idx_t)1);
            sum += t1 * t2;
        }
    }

    return sum;
}
