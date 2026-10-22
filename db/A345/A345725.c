/*
 * A345725  For 1<=x<=n, 1<=y<=n with gcd(x,y)=1, write 1 = gcd(x,y) = u*x+v*y
 *          with u,v minimal; a(n) = m^2*s, where s is the population variance
 *          of the values of u+v and m is the number of such values.
 * https://oeis.org/A345725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ceildiv, floordiv, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computing OEIS sequence A345725.
 * An term a(n) is given by
 *    a(n) = m * sumsq - sum^2
 * where a(n) depends on all coprime pairs (x,y) with 1≤x,y≤n.
 * For each such pair solve 1 = u*x + v*y for integers u,v
 * with minimal |u|+|v|.  Then S = u+v.
 * Let m be the number of coprime pairs (ordered),
 * sum = Σ S over all pairs,
 * sumsq = Σ S^2 over all pairs.
 * The function returns the unsigned type seq_t.
 */

#include <stdint.h>
#include "../utils/ceildiv.h"
#include "../utils/floordiv.h"
#include "../utils/gcd_int.h"

/* small gcd for int arguments */

/* extended Euclidean algorithm: returns gcd of a,b (which should be 1 here)
 * and sets *x,*y such that a*x + b*y = gcd */
static inline int extended_gcd(int a, int b, int *x, int *y)
{
    int old_r = a, r = b, old_s = 1, s = 0, old_t = 0, t = 1;
    while (r != 0) {
        int q = old_r / r;
        int tmp = r;
        r = old_r - q * r;
        old_r = tmp;
        tmp = s;
        s = old_s - q * s;
        old_s = tmp;
        tmp = t;
        t = old_t - q * t;
        old_t = tmp;
    }
    *x = old_s;
    *y = old_t;
    return old_r;                  /* should be 1 for coprime inputs */
}

/* minimal |u|+|v| solution for 1 = u*x + v*y where gcd(x,y)=1
 * returns S = u+v of that solution */
static inline int min_uv_sum(int x, int y)
{
    int u0, v0;
    extended_gcd(x, y, &u0, &v0);      /* u0*x + v0*y = 1 */

    int64_t U = u0, V = v0;
    int64_t a = y, b = x;              /* general solution: u = U + a*k,
                                         v = V - b*k */

    int64_t k_candidates[4];
    k_candidates[0] = floordiv(-U, a);
    k_candidates[1] = ceildiv(-U, a);
    k_candidates[2] = floordiv(V, b);
    k_candidates[3] = ceildiv(V, b);

    int64_t best_k = k_candidates[0];
    int64_t best_f = llabs(U + a * best_k) + llabs(V - b * best_k);

    for (int i = 1; i < 4; ++i) {
        int64_t k = k_candidates[i];
        int64_t f = llabs(U + a * k) + llabs(V - b * k);
        if (f < best_f) {
            best_f = f;
            best_k = k;
        }
    }

    int64_t u = U + a * best_k;
    int64_t v = V - b * best_k;
    return (int)(u + v);
}

seq_t a(idx_t n)
{
    long long sum = 0;          /* ΣS, may be negative */
    long long sumsq = 0;        /* ΣS^2, always non‑negative */
    long long m = 0;            /* number of ordered coprime pairs */

    /* process unordered pairs (x<y) and count both orders */
    for (idx_t ix = 1; ix <= n; ++ix) {
        int x = (int)ix;
        for (idx_t iy = ix + 1; iy <= n; ++iy) {
            int y = (int)iy;
            if (gcd_int(x, y) == 1) {
                int S = min_uv_sum(x, y);
                sum += (long long)S * 2;
                sumsq += ((long long)S * S) * 2;
                m += 2;
            }
        }
    }

    /* handle the self‑pair (1,1) */
    sum += 1;
    sumsq += 1;
    m += 1;

    /* final value: a(n) = m * sumsq - sum^2 */
    seq_t sumsq_seq = (seq_t)sumsq;
    seq_t m_seq     = (seq_t)m;
    seq_t term1     = sumsq_seq * m_seq;

    seq_t sum_abs = (seq_t)(sum >= 0 ? sum : -sum);
    seq_t term2   = sum_abs * sum_abs;

    return term1 - term2;
}
