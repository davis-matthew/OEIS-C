/*
 * A377680  Expansion of e.g.f. (1 + x * (exp(x) - 1))^3.
 * https://oeis.org/A377680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377680: Expansion of e.g.f. (1 + x * (exp(x) - 1))^3
 * a(n) is the coefficient of x^n in
 *     (1 - 24x + 266x^2 - 1815x^3 + 8634x^4 - 30981x^5 + 89318x^6 - 216717x^7
 *      + 446185x^8 - 750291x^9 + 969468x^10 - 907596x^11 + 585648x^12 - 248832x^13
 *      + 64800x^14 - 7776x^15)
 *      divided by ((1 - x)(1 - 2x)(1 - 3x))^4.
 *
 * The coefficients satisfy the linear recurrence
 *
 *   a(n) = p[n] - Σ_{j=1}^{12} q[j] * a(n-j)      (n ≥ 0),
 *
 * where
 *
 *   p[0..15] = {  1,  -24,  266, -1815,  8634, -30981,  89318, -216717,
 *                 446185, -750291, 969468, -907596, 585648, -248832,
 *                 64800, -7776 }
 *
 *   q[0..12] = {  1, -24, 260, -1680,  7206, -21600, 46364, -71760,
 *                79441, -61320, 31320, -9504, 1296 }
 *
 * with q[0] = 1.  For indices outside the defined range the
 * corresponding coefficient is taken as 0.
 *
 * This implementation uses the recurrence above and works
 * for any integer type seq_t as defined by the build
 * (8/16/32/64‑bit unsigned integers or arbitrary precision).
 * It has O(n) time and O(n) space complexity.
 */

#include <stdlib.h>

static const int p[16] = {
    1, -24, 266, -1815, 8634, -30981, 89318, -216717,
    446185, -750291, 969468, -907596, 585648, -248832,
    64800, -7776
};

static const int q[13] = {
    1, -24, 260, -1680, 7206, -21600, 46364, -71760,
    79441, -61320, 31320, -9504, 1296
};

seq_t a(idx_t n)
{
    /* Allocate array for coefficients up to n.  The size is always at least 1. */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a) return 0;   /* out of memory, return 0 (unlikely in typical use) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t term = 0;
        /* Constant term p[i] */
        if (i < 16) {
            int pi = p[i];
            if (pi >= 0)
                term += (seq_t)pi;
            else
                term -= (seq_t)(-pi);   /* subtract abs value of negative pi */
        }
        /* Subtract Σ q[j] * a[i-j]  (j = 1..12, j ≤ i) */
        for (idx_t j = 1; j <= 12 && j <= i; ++j) {
            int qj = q[j];
            seq_t prod = a[i - j] * (seq_t)(qj >= 0 ? qj : -qj);  /* abs(qj) */
            if (qj >= 0)
                term -= prod;
            else
                term += prod;  /* since qj is negative, subtracting qj is adding */
        }
        a[i] = term;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
