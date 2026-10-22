/*
 * A321044  Irregular table related to f[(a*x+b)/(c*x+d)]=(c*x+d)^(2*n)*f[x],
 *          and f[x]=1/(x+1), f[x]=(a*x+b)/(c*x+d).
 * https://oeis.org/A321044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, obsc
 * Formula:   a(n) is given by the coefficients of the polynomials P_r(x)
 *            defined by P_1(x) = 1 + x P_r(x) = (1 + 2x + x^2) * P_{r-1}(x) +
 *            (2x + x^2) for r >= 2 where a(n) is the coefficient of x^k in
 *            P_r(x) with r, k determined by n = 1 + (r-1)r + k, 0 ≤ k ≤ 2r-1,
 *            and a(0) = 1. This yields row sums 3*2^(2r-2)...
 * Range:     u8 n <= 33, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

/* compute C(cur_n+2, m) given current value C(cur_n, m) for even cur_n */
static seq_t next_binom(seq_t cur, idx_t cur_n, idx_t m) {
    if (m == 0) return 1;
    if (cur == 0) {
        /* still zero until cur_n+2 >= m */
        if (cur_n + 2 < m) return 0;
        return binom(cur_n + 2, m);
    }
    /* cur != 0, use recurrence */
    seq_t t = cur * (seq_t)(cur_n + 1) / (seq_t)(cur_n + 1 - m);
    t = t * (seq_t)(cur_n + 2) / (seq_t)(cur_n + 2 - m);
    return t;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 */

    /* find row r such that 1 + (r-1)r <= n < 1 + r(r+1) */
    idx_t r = 1;
    while ((idx_t)1 + r * (r + 1) <= n) ++r;

    /* offset within the row */
    idx_t low = ((idx_t)1 + (r - 1) * r);
    idx_t k = n - low;            /* 0 <= k <= 2r-1 */

    /* main binomial coefficient */
    seq_t main = binom(2 * r - 1, k);

    seq_t sum1 = 0, sum2 = 0;
    if (r <= 1) return main;      /* no internal sums for r = 1 */

    idx_t max_t = r - 2;          /* number of t values */
    seq_t c1 = (k >= 1 && (k - 1) == 0) ? 1 : 0;   /* C(0, k-1) */
    seq_t c2 = (k >= 2 && (k - 2) == 0) ? 1 : 0;   /* C(0, k-2) */

    for (idx_t t = 0; t <= max_t; ++t) {
        if (k >= 1) sum1 += c1;
        if (k >= 2) sum2 += c2;

        if (t == max_t) break;

        idx_t cur_n = 2 * t;
        if (k >= 1) c1 = next_binom(c1, cur_n, k - 1);
        if (k >= 2) c2 = next_binom(c2, cur_n, k - 2);
    }

    seq_t result = main;
    if (k >= 1) result += (seq_t)2 * sum1;
    if (k >= 2) result += sum2;
    return result;
}
