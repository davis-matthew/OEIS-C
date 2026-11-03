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
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the minimal‑absolute coefficient of x in the
   Bézout identity for the pair (x,y). */
static long long minimal_u_signed(int x, int y)
{
    int a = x, b = y;
    int u0 = 1, v0 = 0;   /* coefficients for a, b          */
    int u1 = 0, v1 = 1;   /* previous coefficients          */

    while (b != 0) {
        int q = a / b;
        int r = a - q * b;
        int u2 = u0 - q * u1;
        int v2 = v0 - q * v1;
        a = b; b = r;
        u0 = u1; u1 = u2;
        v0 = v1; v1 = v2;
    }

    int g   = a;           /* gcd(x,y)           */
    int x1  = x / g;       /* reduced numbers    */
    int y1  = y / g;
    int u0v = u0;          /* coefficient for the reduced x */

    /* minimise |u0v| over the one‑parameter family */
    int d = y1;
    int r = u0v % d;
    if (r < 0) r += d;     /* 0 <= r < d            */
    int u = (r <= d - r) ? r : r - d;  /* minimal |u|       */

    return (long long)u;
}

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
