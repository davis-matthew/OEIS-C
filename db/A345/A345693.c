/*
 * A345693  For 1<=x<=n, 1<=y<=n with gcd(x,y)=1, write 1 = gcd(x,y) = u*x+v*y
 *          with u,v minimal; a(n) = m^2*s, where s is the population variance
 *          of the values of v and m is the number of such values.
 * https://oeis.org/A345693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m^2 * s where s is the population variance of the *
 *            values of v for all coprime pairs (x,y) with 1 ≤ x ≤ n, 1 ≤ y ≤
 *            n. * Here m is the number of such pairs
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   ext_gcd_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ext_gcd_signed.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = b;
        b   = a % b;
        a   = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* accumulate sums */
    long long  sum_v  = 0;            /* can be negative */
    unsigned long long sum_v2 = 0;    /* squared values, always non‑negative */
    unsigned long long m      = 0;    /* number of coprime pairs */

    for (idx_t x = 1; x <= n; ++x)
    {
        for (idx_t y = 1; y <= n; ++y)
        {
            if (gcd_idx(x, y) != 1)
                continue;

            /* compute a particular Bézout solution */
            long long u0, v0;
            ext_gcd_signed((long long)x, (long long)y, &u0, &v0);

            /* bring u0 into the range (-y/2, y/2] */
            long long y_ll = (long long)y;
            long long r = u0 % y_ll;
            if (r < 0)
                r += y_ll;                 /* now 0 <= r < y */
            if (r > y_ll / 2)
                r -= y_ll;                 /* now -y/2 < r <= y/2 */

            /* corresponding v */
            long long v = (1 - r * (long long)x) / y_ll;  /* exact division */

            sum_v  += v;
            sum_v2 += (unsigned long long)(v * v);
            ++m;
        }
    }

    /* a(n) = m * sum(v^2) - (sum(v))^2   (exact integer) */
    unsigned __int128 big = (unsigned __int128)m * sum_v2
                           - (unsigned __int128)sum_v * sum_v;

    return (seq_t)big;
}
