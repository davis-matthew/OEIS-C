/*
 * A345724  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = n^4*s, where s is the population variance of the values of
 *          u+v.
 * https://oeis.org/A345724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 * s, where s is the population variance of the values
 *            of u+v for all pairs 1≤x,y≤n, obtained from the minimal Bézout
 *            representation gcd(x,y)=u*x+v*y. The implementation computes the
 *            required sums directly
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd, min_abs_sum_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/min_abs_sum_candidate.h"

static inline int egcd(int a, int b, int *x, int *y)
/* extended Euclid – returns gcd(a,b); finds integers x,y with ax+by=gcd */
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    int g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

seq_t a(idx_t n)
{
    /* use signed 128‑bit integers for intermediate sums */
    __int128_t sum1 = 0;   /* Σ (u+v) */
    __int128_t sum2 = 0;   /* Σ (u+v)^2 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;          /* floor(n/k)  */
        if (k == 1) {
            /* only pair (1,1) on the diagonal – value u+v = 1 */
            int t = 1;
            sum1 += (__int128_t)m * t;
            sum2 += (__int128_t)m * (t * t);
            continue;
        }

        for (idx_t b = 1; b < k; ++b) {
            if (gcd((seq_t)k, (seq_t)b) != 1) continue;   /* only coprime pairs */

            int a_int = (int)k;
            int b_int = (int)b;

            /* find one Bézout solution for a*u + b*v = 1 */
            int u0, v0;
            egcd(a_int, b_int, &u0, &v0);

            /* bring u into [0, b-1] */
            int r = u0 % b_int;
            if (r < 0) r += b_int;

            int u1 = r;
            int v1 = (1 - a_int * u1) / b_int;

            int u_sel, v_sel;
            min_abs_sum_candidate(a_int, b_int, u1, v1, &u_sel, &v_sel);

            int t_val = u_sel + v_sel;   /* u+v for this coprime pair */

            /* contributions from both orders (k,b) and (b,k) */
            sum1 += (__int128_t)m * 2 * (int64_t)t_val;
            sum2 += (__int128_t)m * 2 * (int64_t)(t_val * t_val);
        }
    }

    /* a(n) = n^2 * Σ s_i^2 – (Σ s_i)^2  */
    __int128_t n2 = (__int128_t)n * (__int128_t)n;
    __int128_t ans = n2 * sum2 - sum1 * sum1;

    return (seq_t)ans;
}
