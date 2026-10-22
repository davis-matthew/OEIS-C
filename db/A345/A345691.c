/*
 * A345691  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = n^4*s, where s is the population variance of the values of
 *          u^2+v^2.
 * https://oeis.org/A345691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * Σ_{x=1}^{n} Σ_{y=1}^{n} (u_{x,y}^2+v_{x,y}^2)^2 - (
 *            Σ_{x=1}^{n} Σ_{y=1}^{n} (u_{x,y}^2+v_{x,y}^2) )^2 where gcd(x,y)
 *            = u_{x,y}·x + v_{x,y}·y with u_{x,y},v_{x,y} minimal
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_int.h"   /* provides int gcd_int(int a,int b) */

static void extended_gcd(int64_t a, int64_t b, int64_t *u, int64_t *v, int64_t *g) {
    int64_t r0 = a, r1 = b;
    int64_t s0 = 1, s1 = 0;
    int64_t t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;   r0 = r1; r1 = r2;
        int64_t s2 = s0 - q * s1;   s0 = s1; s1 = s2;
        int64_t t2 = t0 - q * t1;   t0 = t1; t1 = t2;
    }
    *g = r0;
    *u = s0;
    *v = t0;
}

seq_t a(idx_t n) {
    seq_t sum1 = 0;      /* Σ t */
    seq_t sum2 = 0;      /* Σ t² */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sumk_t  = 0;
        seq_t sumk_t2 = 0;

        if (k == 1) {                /* only pair (1,1) */
            sumk_t  = 1;                 /* u=1,v=0 → u²+v² = 1 */
            sumk_t2 = 1;
        } else {
            for (idx_t b = 1; b < k; ++b) {
                if (gcd_int((int)k, (int)b) != 1) continue;   /* skip non‑coprime */
                int64_t u, v, g;
                extended_gcd((int64_t)k, (int64_t)b, &u, &v, &g);
                if (g != 1) continue;          /* safety, should never happen */
                int64_t t = u * u + v * v;
                seq_t t_seq = (seq_t)t;
                sumk_t  += t_seq;
                sumk_t2 += t_seq * t_seq;
            }
            sumk_t  *= 2;                      /* (+b,k) also contributes */
            sumk_t2 *= 2;
        }

        seq_t factor = (seq_t)(n / k);        /* floor(n/k) */
        sum1 += sumk_t  * factor;
        sum2 += sumk_t2 * factor;
    }

    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t result = n_sq * sum2 - sum1 * sum1;
    return result;
}
