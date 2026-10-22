/*
 * A352687  Triangle read by rows, a Narayana related triangle whose rows are
 *          refinements of twice the Catalan numbers (for n >= 2).
 * https://oeis.org/A352687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^r in the polynomial P(t,x), where t
 *            is the largest integer with t(t+1)/2 ≤ n. a(n)=0 when n is a
 *            triangular number. P(n,x) generates the Narayana‑related
 *            triangle: P(0)=1, P(1)=x,
 *            P(n)=((2n-3)(x+1)P(n-1)-(n-3)(x-1)^2P(n-2))/n for n≥2. The
 *            coefficient of x^k in P(n,x)...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t T_n_k(idx_t n, idx_t k) {
    /* For 1 ≤ k ≤ n−1;  T(0,0)=1 is handled elsewhere. */
    seq_t term1 = binomial((idx_t)n, (idx_t)k);
    term1 *= binomial((idx_t)n - 1, (idx_t)k - 1);
    term1 /= (seq_t)(n - k + 1);

    seq_t term2 = 0;
    if (k >= 2 && k <= n - 1) {
        term2 = 2 * binomial((idx_t)n - 1, (idx_t)k);
        term2 *= binomial((idx_t)n - 1, (idx_t)k - 2);
        term2 /= (seq_t)(n - 1);
    }

    return term1 - term2;
}

seq_t a(idx_t n) {
    /* Special case n=0 */
    if (n == 0) return 1;

    /* Find the largest t with T(t)=t(t+1)/2 <= n */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) t++;

    /* Check whether n is a triangular number */
    idx_t tri = t * (t + 1) / 2;
    if (n == tri) return 0;

    /* r = n - T(t) ranges from 1 to t */
    idx_t r = n - tri;

    /* Return the coefficient T(t,r) */
    return T_n_k(t, r);
}
