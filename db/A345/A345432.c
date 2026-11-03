/*
 * A345432  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of |u|.
 * https://oeis.org/A345432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1<=x,y<=n} |u|, where gcd(x,y) = u*x+v*y with u,v
 *            chosen to minimize u^2+v^2. For x=y the special rule u=0,v=1 is
 *            used
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd_idx.h"

/* Extended GCD on integers: returns u,v such that a*u+b*v=gcd(a,b) */
static inline void _egcd(int a, int b, long long *u, long long *v)
{
    long long x0 = 1, y0 = 0;
    long long x1 = 0, y1 = 1;
    while (b != 0) {
        int q = a / b;
        int t = a % b;
        a = b; b = t;
        long long tmp = x0 - (long long)q * x1;
        x0 = x1; x1 = tmp;
        tmp = y0 - (long long)q * y1;
        y0 = y1; y1 = tmp;
    }
    *u = x0;
    *v = y0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i + 1; j <= n; ++j) {
            idx_t g = gcd_idx(i, j);
            /* Reduce to coprime pair */
            int X = (int)(i / g);
            int Y = (int)(j / g);

            long long u, v;
            _egcd(X, Y, &u, &v);

            long long absu = u >= 0 ? u : -u;
            long long absv = v >= 0 ? v : -v;

            total += (seq_t)absu + (seq_t)absv;
        }
    }
    return total;
}
