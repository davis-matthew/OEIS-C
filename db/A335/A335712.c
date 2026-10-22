/*
 * A335712  The sum of the sizes of the minimal fixed points over all
 *          compositions of n.
 * https://oeis.org/A335712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j>=1} j * [coeff of x^{n-j} in * (Product_{i=1..j-1}
 *            (x/(1-x)-x^i)) * (1-x)/(1-2x) ] * This implementation evaluates
 *            the generating‑function * formula by dynamic programming in
 *            O(n²) time. * The algorithm uses only +, -, *, <<, >>, and
 *            integer arithmetic * on the type seq_t
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Allocate space for polynomials and answer */
#ifdef __cplusplus
    std::vector<seq_t> P(n + 1, seq_t());
    std::vector<seq_t> Pnext(n + 1, seq_t());
    std::vector<seq_t> ans(n + 1, seq_t());
    P[0] = seq_t(1);                // polynomial for j = 1 (empty product)
#else
    seq_t *P = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *Pnext = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *ans = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) {
        P[i] = 0;
        Pnext[i] = 0;
        ans[i] = 0;
    }
    P[0] = (seq_t)1;
#endif

    for (idx_t j = 1; j <= n; ++j) {
        idx_t up = n - j;               /* maximal index of convolution term */

        /* convolution of current polynomial P with H(x) = (1-x)/(1-2x)
           running via recurrence: conv[t] = P[t] + 2*conv[t-1] - P[t-1]   */
        seq_t conv_prev = P[0];

        /* contribution for n = j (t = 0) */
        ans[j] += (seq_t)j * conv_prev;

        for (idx_t t = 1; t <= up; ++t) {
            seq_t two_prev = conv_prev + conv_prev;
            seq_t conv_cur = P[t] + two_prev - P[t - 1];
            ans[j + t] += (seq_t)j * conv_cur;
            conv_prev = conv_cur;
        }

        /* Prepare polynomial for the next j (increase product by factor
           (x/(1-x)-x^j)) if more iterations will follow. */
        if (j < n) {
            idx_t next_deg = n - (j + 1);
            seq_t pref = (seq_t)0;
            for (idx_t s = 0; s <= next_deg; ++s) {
                if (s > 0) pref += P[s - 1];
                seq_t sub = (s >= j) ? P[s - j] : (seq_t)0;
                Pnext[s] = pref - sub;
            }
#ifdef __cplusplus
            for (idx_t s = 0; s <= next_deg; ++s) P[s] = Pnext[s];
#else
            for (idx_t s = 0; s <= next_deg; ++s) P[s] = Pnext[s];
#endif
        }
    }

    return ans[n];
}
