/*
 * A345687  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = n^4*s, where s is the population variance of the values of
 *          u.
 * https://oeis.org/A345687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * sum(u^2) - (sum(u))^2 where u is the coefficient of
 *            x in the Bézout identity u*x + v*y = gcd(x,y) chosen to minimise
 *            |u|
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   gcd, minimal_u_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/minimal_u_signed.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    long long sum_u  = 0;
    unsigned long long sum_u2 = 0;

    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            long long u = minimal_u_signed((int)x, (int)y);
            sum_u += u;
            sum_u2 += (unsigned long long)(u * u);
        }
    }

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;                 /* n^2                */
    seq_t term1 = n2 * (seq_t)sum_u2;    /* n^2 * sum(u^2)     */

    __int128 sumu_sq128 = (__int128)sum_u * sum_u;
    seq_t sumu_sq = (seq_t)sumu_sq128;   /* (sum(u))^2          */

    seq_t a_val = term1 - sumu_sq;
    return a_val;
}
