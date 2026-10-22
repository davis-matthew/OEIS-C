/*
 * A345689  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = n^4*s, where s is the population variance of the values of
 *          |u|.
 * https://oeis.org/A345689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * sum(|u|^2) - (sum |u|)^2 where u is the minimal
 *            absolute coefficient of x in the Bézout identity gcd(x,y) = u*x
 *            + v*y
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- helpers ---------- */
static inline idx_t gcd_uint64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Extended Euclid for 64‑bit signed integers.
   Returns gcd(a,b) and puts coefficients in *x,*y such that
   a*x + b*y = gcd. */
static void extended_gcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t old_r = a, r = b;
    int64_t old_s = 1, s = 0;
    int64_t old_t = 0, t = 1;
    while (r != 0) {
        int64_t q = old_r / r;
        int64_t tmp = r;
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
    /* gcd is old_r */
}

static seq_t abs_seq(seq_t v) {
    /* v is non–negative; just return */
    return v;
}

/* ---------- main function ---------- */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* arrays for sums per maximal value b */
    size_t N = (size_t)n + 1;
    seq_t *sum_u  = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *sum_u2 = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *sum_v  = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *sum_v2 = (seq_t *)calloc(N, sizeof(seq_t));

    /* compute sums for coprime pairs (a,b), a < b */
    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = a + 1; b <= n; ++b) {
            if (gcd_uint64(a, b) != 1) continue;

            int64_t x, y;          /* extended gcd coefficients */
            extended_gcd((int64_t)a, (int64_t)b, &x, &y);
            /* x is coefficient for a, y for b */
            int64_t abs_x = x >= 0 ? x : -x;
            int64_t abs_y = y >= 0 ? y : -y;

            seq_t ux = (seq_t)abs_x;
            seq_t vx = (seq_t)abs_y;

            sum_u[b]  += ux;
            sum_u2[b] += ux * ux;
            sum_v[b]  += vx;
            sum_v2[b] += vx * vx;
        }
    }

    /* accumulate weighted sums */
    seq_t S1 = 0;
    seq_t S2 = 0;
    for (idx_t b = 1; b <= n; ++b) {
        idx_t k = n / b;  /* number of multiples g */
        seq_t mul = (seq_t)k;
        S1 += mul * (sum_u[b] + sum_v[b]);
        S2 += mul * (sum_u2[b] + sum_v2[b]);
    }

    /* a(n) = n^2 * S2 - S1^2 */
    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t result = n2 * S2 - S1 * S1;

    /* free memory */
    free(sum_u);
    free(sum_u2);
    free(sum_v);
    free(sum_v2);

    return result;
}
