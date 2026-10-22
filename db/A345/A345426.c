/*
 * A345426  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of u.
 * https://oeis.org/A345426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{x=1}^n ∑_{y=1}^n u(x,y) where u(x,y) is the
 *            coefficient of x in the Bézout identity gcd(x,y) = u*x + v*y
 *            with |u|^2+|v|^2 minimal (u=0,v=1 when x=y)
 * Range:     i8 n <= 49, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   minimal_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/minimal_u.h"
/* helper: gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            idx_t g = gcd_idx(x, y);
            idx_t a1 = x / g;
            idx_t b1 = y / g;
            int u = minimal_u((int)a1, (int)b1);
            sum += (seq_t)u;
        }
    }
    return sum;
}
