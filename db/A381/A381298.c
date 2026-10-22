/*
 * A381298  a(n) = denominator( [x^n] hypergeom([1/2, 1/2, 1/2, 1/4, 3/4], [1,
 *          1, 1, 1], 256*x) ).
 * https://oeis.org/A381298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( 4*(4*n -
 *            1)!*Gamma(n+1/2)^2/(Pi*(n-1)!*(n!)^5) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplifies to a(n) = 2^e(n), where e(n) = max(0, 4n + v2((n-1)!)+7v2(n!) -
                                   (2 + v2((4n-1)!)+2v2((2n)!))) */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t v2_factorial(idx_t m)
{
    idx_t sum = 0;
    for (idx_t x = m; x; x >>= 1)
        sum += x >> 1;          /* add floor(x/2) for each k */
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return pow2(0);

    idx_t a_fact = 4*n - 1;    /* (4n-1)!   */
    idx_t b_fact = 2*n;        /* (2n)!     */
    idx_t c_fact = n - 1;      /* (n-1)!    */
    idx_t d_fact = n;          /* (n)!      */

    idx_t numer2 = 2 + v2_factorial(a_fact) + 2 * v2_factorial(b_fact);
    idx_t denom2 = 4*n + v2_factorial(c_fact) + 7 * v2_factorial(d_fact);

    idx_t e = (denom2 > numer2) ? (denom2 - numer2) : 0;
    return pow2(e);
}
