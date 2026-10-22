/*
 * A345423  For 1<=x<=n, 1<=y<=n, with gcd(x,y)=1, write 1 = gcd(x,y) =
 *          u*x+v*y with u,v minimal; a(n) = sum of the values of u.
 * https://oeis.org/A345423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 42, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A345423:
   a(n) = Σ_{1≤x,y≤n, gcd(x,y)=1} u(x,y)
   where integer pair (u,v) satisfies 1 = u·x + v·y
   and (u,v) has minimal Euclidean norm u²+v².
   For x = y the assignment is set to u = 0, v = 1.
*/

#include <stdint.h>   /* for uint64_t */

/* Extended Euclidean algorithm.
   Computes g = gcd(a,b) and coefficients x,y such that g = a*x + b*y. */
static void extended_gcd(seq_t a, seq_t b, seq_t *g, seq_t *x, seq_t *y)
{
    seq_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        seq_t q = a / b;
        seq_t r = a - q * b;          /* exact remainder */
        seq_t x2 = x0 - q * x1;
        seq_t y2 = y0 - q * y1;
        a = b;
        b = r;
        x0 = x1;
        x1 = x2;
        y0 = y1;
        y1 = y2;
    }
    *g = a;
    *x = x0;
    *y = y0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    seq_t sum = 0;

    for (idx_t y = 2; y <= n; ++y) {
        seq_t y_t = (seq_t)y;
        for (idx_t x = 1; x < y; ++x) {
            seq_t x_t = (seq_t)x;
            seq_t g, u0, v0;
            extended_gcd(x_t, y_t, &g, &u0, &v0);
            if (g != (seq_t)1) continue;   /* not coprime */

            /* Reduce u0 modulo y to obtain the minimal‑norm coefficient u. */
            seq_t r = u0 % y_t;
            if (r < (seq_t)0) r += y_t;      /* ensure 0 <= r < y */
            seq_t half = y_t / 2;
            seq_t u = (r > half) ? (r - y_t) : r;

            /* Compute the corresponding v for this minimal (u,v). */
            seq_t t = (u - u0) / y_t;          /* integer division, exact */
            seq_t v = v0 - t * x_t;

            /* The ordered pair (x,y) contributes u,
               the pair (y,x) contributes v.  Add both. */
            sum += u + v;
        }
    }

    return sum;
}
