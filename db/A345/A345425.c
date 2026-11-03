/*
 * A345425  For 1<=x<=n, 1<=y<=n, with gcd(x,y)=1, write 1 = gcd(x,y) =
 *          u*x+v*y with u,v minimal; a(n) = sum of the values of u+v.
 * https://oeis.org/A345425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{1<=x,y<=n, gcd(x,y)=1} (u(x,y)+v(x,y)) where
 *            u*x+v*y=1 is the solution with minimal u^2+v^2. (x=y=1 is
 *            treated specially: u=0, v=1)
 * Range:     i8 n <= 33, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Extended Euclid: returns gcd and sets *x,*y so that a*x + b*y = gcd(a,b) */
static inline int64_t extgcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b) {
        int64_t q = a / b;
        int64_t t = a % b;  a = b; b = t;
        t = x0 - q * x1;   x0 = x1; x1 = t;
        t = y0 - q * y1;   y0 = y1; y1 = t;
    }
    *x = x0;
    *y = y0;
    return a;
}

/* round N/den to nearest integer */
static inline int64_t round_div(__int128 N, __int128 den) {
    if (N >= 0)
        return (int64_t)((N + den/2) / den);
    else
        return -(int64_t)((-N + den/2) / den);
}

/* Euclidean gcd on int64_t */
static inline int64_t my_gcd(int64_t a, int64_t b) {
    while (b) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t ix = 1; ix <= n; ++ix) {
        int64_t x = (int64_t)ix;
        for (idx_t jy = 1; jy <= n; ++jy) {
            int64_t y = (int64_t)jy;
            /* check coprimality */
            if (my_gcd(x, y) != 1) continue;

            /* particular solution (u0,v0) of x*u0 + y*v0 = 1 */
            int64_t u0, v0;
            extgcd(x, y, &u0, &v0);

            /* find integer k that minimizes u^2+v^2 */
            __int128 N = - (__int128)y * u0 + (__int128)x * v0;
            __int128 den = (__int128)x * x + (__int128)y * y;
            int64_t k = round_div(N, den);

            int64_t u = u0 + k * y;
            int64_t v = v0 - k * x;
            sum += (seq_t)(u + v);
        }
    }
    return sum;
}
