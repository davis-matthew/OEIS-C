/*
 * A345690  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = n^4*s, where s is the population variance of the values of
 *          |v|.
 * https://oeis.org/A345690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=n^4*s where s is the population variance of the values of
 *            |v| * for all pairs (x,y) with 1≤x,y≤n. |v| is defined as the
 *            coefficient * in the Bézout relation gcd(x,y)=u*x+v*y that is
 *            minimal in absolute * value subject to |u|<y/gcd(x,y) and
 *            |v|<x/gcd(x,y) which is the unique * minimal represe...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Extended Euclidean algorithm returning gcd and one solution (u,v). +signed */
static inline int64_t egcd(int64_t a, int64_t b, int64_t *u, int64_t *v)
{
    int64_t old_r = a, r = b;
    int64_t old_s = 1, s = 0;
    int64_t old_t = 0, t = 1;
    while (r != 0) {
        int64_t q = old_r / r;
        int64_t tmp = old_r - q * r; old_r = r; r = tmp;
        tmp = old_s - q * s; old_s = s; s = tmp;
        tmp = old_t - q * t; old_t = t; t = tmp;
    }
    *u = old_s;
    *v = old_t;
    return old_r;            /* gcd */
}

/* floor division for signed a, positive b */
static inline int64_t floor_div(int64_t a, int64_t b)
{
    if (b <= 0) return 0;   /* not expected */
    if (a >= 0) return a / b;
    return - ((-a + b - 1) / b);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe guard */

    /* accumulate sums as big integers (__int128) */
    __int128 S1 = 0;          /* Σ |v|  */
    __int128 S2 = 0;          /* Σ |v|² */

    for (idx_t xi = 1; xi <= n; ++xi) {
        int64_t x = (int64_t)xi;
        for (idx_t yj = 1; yj <= n; ++yj) {
            int64_t y = (int64_t)yj;

            int64_t u0, v0;
            int64_t g = egcd(x, y, &u0, &v0);      /* g > 0 */

            int64_t d  = y / g;      /* y/g   */
            int64_t a_d = x / g;      /* x/g   */

            /* Find k such that u = u0 + k*d satisfies |u| < d.
               Two candidates (k and k+1) are enough. */
            int64_t k1 = floor_div(-u0, d);
            int64_t best_abs = INT64_MAX;

            for (int c = 0; c < 2; ++c) {
                int64_t k = k1 + c;
                int64_t u = u0 + k * d;
                if (u <= -d || u >= d) continue;            /* out of bounds */

                int64_t v = v0 - k * a_d;                   /* corresponding v */
                int64_t abs_v = v >= 0 ? v : -v;
                if (abs_v < best_abs) best_abs = abs_v;
            }

            /* best_abs now holds minimal |v| for this pair */
            S1 += best_abs;
            S2 += (__int128)best_abs * best_abs;
        }
    }

    __int128 N2 = (__int128)n * (__int128)n;   /* n²  */
    __int128 val = N2 * S2 - S1 * S1;          /* a(n)  */

    return (seq_t)val;
}
