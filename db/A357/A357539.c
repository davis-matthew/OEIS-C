/*
 * A357539  a(n) = coefficient of x^n/n! in: Sum_{n>=0} ( x*exp(x)
 *          )^(n*(n+1)/2).
 * https://oeis.org/A357539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ∑_{k≥0, t=k(k+1)/2 ≤ n} (n!/(n−t)!) * t^{\,n−t}
   where t = k(k+1)/2.  This implements the definition
   of OEIS A357539 from the exponential generating function:
   A(x) = Σ_{k≥0} (x·e^x)^{k(k+1)/2}.  The computation uses only
   integer arithmetic and works for any seq_t size. */

/* Standard integer types for indexes */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; ; ++k)
    {
        /* triangular number t = k(k+1)/2 */
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;          /* no more terms contribute */

        idx_t m = n - t;            /* exponent for t^m */

        /* falling product n!/(m)! = product_{i=m+1}^{n} i */
        seq_t fall = 1;
        for (idx_t i = m + 1; i <= n; ++i)
            fall *= (seq_t)i;

        /* power t^m */
        seq_t pow = 1;
        for (idx_t i = 0; i < m; ++i)
            pow *= (seq_t)t;

        sum += fall * pow;
    }

    return sum;
}
