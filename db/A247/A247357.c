/*
 * A247357  Least m such that (4/m^2)*Sum_{k=0..m} sqrt(m^2 - k^2) < Pi + 1/n.
 * https://oeis.org/A247357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m such that (4/m^2)*Sum_{k=0..m} sqrt(m^2 - k^2) <
 *            pi + 1/n
 * Range:     u8 n <= 132, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static const int MAX_M = 25000;          /* safe upper bound for a(10000)   */
static double *expr_tbl = NULL;          /* pre‑computed values of 4S(m)/m^2 */

static void init_expr(void)
{
    expr_tbl = (double *)malloc((MAX_M + 1) * sizeof(double));
    if (!expr_tbl) exit(1);              /* allocation failure is fatal */

    expr_tbl[0] = 0.0;
    for (int m = 1; m <= MAX_M; ++m) {
        double sum = 0.0;
        double m2  = (double)m * (double)m;
        for (int k = 0; k <= m; ++k) {
            double t = m2 - (double)k * (double)k;
            sum += sqrt(t);                  /* sqrt is of type double          */
        }
        expr_tbl[m] = 4.0 * sum / m2;          /* 4S(m)/m^2                         */
    }
}

seq_t a(idx_t n)
{
    if (!expr_tbl) init_expr();                /* one‑time initialization          */

    const long double PI = acosl(-1.0L);
    long double target = PI + 1.0L / (long double)n; /* required bound             */
    double tgt = (double)target;              

    int lo = 1, hi = MAX_M;
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        if (expr_tbl[mid] < tgt)               /* strict inequality required       */
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;                          /* guaranteed within seq_t range    */
}
