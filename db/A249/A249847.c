/*
 * A249847  Number of length 3+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + (6/5)n^6 + 3*n^5 + 3*n^4 + (5/6)n^3 + (4/5)n^2 +
 *            (1/6)n This is equivalent to the rational generating function
 *            G(x) = x*(10 + 279*x + 1472*x^2 + 2186*x^3 + 990*x^4 + 103*x^5)
 *            / (1 - x)^8. It can be evaluated by the binomial-hypergeometric
 *            form a(n) = Σ_{i=1}^{6} p_i * C(n - i + 7, 7)...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Coefficients p_i for the binomial sum */
    static const int p[7] = {0, 10, 279, 1472, 2186, 990, 103};
    const int k = 7;                /* binomial exponent */
    seq_t result = 0;

    if (n < 1) {
        return 0;                   /* undefined for indices < 1 */
    }

    for (int i = 1; i <= 6; ++i) {
        /* Compute a = n - i + 7 */
        idx_t a_val = n - i + 7;

        if (a_val < (idx_t)k) {
            /* C(a_val, k) = 0 for a_val < k */
            continue;
        }

        /* Compute C(a_val, k) via multiplicative formula */
        seq_t binom = 1;
        for (int j = 1; j <= k; ++j) {
            /* binom = binom * (a_val - k + j) / j; */
            binom = binom * (a_val - k + j) / j;
        }

        /* Add contribution p_i * binom to the result */
        result += (seq_t)p[i] * binom;
    }

    return result;
}
