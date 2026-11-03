/*
 * A345427  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of v.
 * https://oeis.org/A345427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{1≤x≤n, 1≤y≤n} v(x,y) where v is the Bézout coefficient
 *            minimizing u²+v² in gcd(x,y)=u*x+v*y and u=0, v=1 for x=y
 * Range:     i8 n <= 54, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

/* Extended Euclid: returns gcd of a,b and finds (s,t) with a*s + b*t = gcd */
static void _extgcd(long long a, long long b, long long *s, long long *t)
{
    long long s0 = 1, t0 = 0;
    long long s1 = 0, t1 = 1;
    while (b != 0) {
        long long q = a / b;
        long long r = a % b;

        long long ns = s1;
        long long nt = t1;

        s1 = s0 - q * s1;
        t1 = t0 - q * t1;

        s0 = ns;
        t0 = nt;

        a = b;
        b = r;
    }
    *s = s0;
    *t = t0;
}

/* Euclidean gcd on unsigned 64‑bit indices */

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            if (x == y) {
                /* u = 0, v = 1 by convention */
                sum += 1;
                continue;
            }

            /* gcd(x,y) */
            idx_t d = gcd_idx(x, y);
            /* coprime pair */
            long long X = (long long)(x / d);
            long long Y = (long long)(y / d);

            /* one solution of X*s + Y*t = 1 */
            long long s0, t0;
            _extgcd(X, Y, &s0, &t0);

            /* choose k that minimizes s^2+t^2: k = round( (X*t0 - Y*s0) / (X^2+Y^2) ) */
            __int128 num = (__int128)X * t0 - (__int128)Y * s0;
            __int128 den = (__int128)X * X + (__int128)Y * Y;
            long long k;
            if (num >= 0)
                k = (long long)((num + den / 2) / den);
            else
                k = (long long)((num - den / 2) / den);

            long long s = s0 + k * Y;
            long long t = t0 - k * X;

            /* v is the coefficient for y in the Bézout representation */
            sum += (seq_t)t;
        }
    }
    return sum;
}
