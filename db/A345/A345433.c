/*
 * A345433  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of |v|.
 * https://oeis.org/A345433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1<=x<=n,1<=y<=n} |v(x,y)| where gcd(x,y)=u*x+v*y,
 *            (u,v) is the minimal‑norm solution and for x=y we set u=0,v=1
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Extended Euclidean algorithm. For given a,b computes integers x,y such that
   x*a + y*b = g = gcd(a,b).  For coprime a,b, g==1 and y is the inverse of b
   modulo a, x is the inverse of a modulo b. */
static inline void ext_gcd(idx_t a, idx_t b,
                           int64_t *x, int64_t *y, idx_t *g)
{
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    int64_t aa = (int64_t)a, bb = (int64_t)b;
    while (bb != 0) {
        int64_t q = aa / bb;
        int64_t r = aa - q * bb;
        int64_t x2 = x0 - q * x1;
        int64_t y2 = y0 - q * y1;
        aa = bb; bb = r;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }
    *g = (idx_t)aa;
    *x = x0;
    *y = y0;
}

/* For coprime a,b>1 returns (inv_b_mod_a, inv_a_mod_b).  If a or b is 1,
   the caller must handle those cases separately. */
static inline int inverses_of_mod(idx_t a, idx_t b,
                                  idx_t *inv_b_mod_a, idx_t *inv_a_mod_b)
{
    int64_t x, y;
    idx_t g;
    ext_gcd(a, b, &x, &y, &g);
    if (g != 1) return 0;
    int64_t invb = y % (int64_t)a;
    if (invb < 0) invb += (int64_t)a;
    int64_t inva = x % (int64_t)b;
    if (inva < 0) inva += (int64_t)b;
    *inv_b_mod_a = (idx_t)invb;
    *inv_a_mod_b = (idx_t)inva;
    return 1;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)n;                /* (1,1) pairs, each contributes 1 */
    for (idx_t m = 2; m <= n; ++m) {
        idx_t count = n / m;
        if (count == 0) continue;

        /* pair (m,1) contributes |v| = 1 */
        sum += (seq_t)count * (seq_t)1;

        for (idx_t b = 2; b < m; ++b) {
            idx_t inv_b_mod_m, inv_a_mod_b;
            if (!inverses_of_mod(m, b, &inv_b_mod_m, &inv_a_mod_b)) continue; /* not coprime */

            /* |v| for (a=m,b)  */
            idx_t v_ab = inv_b_mod_m;
            if (m - inv_b_mod_m < v_ab) v_ab = m - inv_b_mod_m;

            /* |v| for (a=b,b=m) */
            idx_t v_ba = inv_a_mod_b;
            if (b - inv_a_mod_b < v_ba) v_ba = b - inv_a_mod_b;

            sum += (seq_t)count * ((seq_t)v_ab + (seq_t)v_ba);
        }
    }
    return sum;
}
