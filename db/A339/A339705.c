/*
 * A339705  Dirichlet g.f.: Product_{k>=2} (1 - k^(-s))^6.
 * https://oeis.org/A339705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet g.f.: ∏_{k≥2} (1 - k^{-s})^6.
   Computes the Dirichlet coefficient a(n) of that product
   using an incremental convolution over all k up to n.
   The coefficients for prime powers are given implicitly,
   but this algorithm works for all n. */
#include <stdlib.h>

static const seq_t COEFF[7] = {
    (seq_t)1,          /* e = 0  */
    -(seq_t)6,         /* e = 1  */
    (seq_t)15,         /* e = 2  */
    -(seq_t)20,        /* e = 3  */
    (seq_t)15,         /* e = 4  */
    -(seq_t)6,         /* e = 5  */
    (seq_t)1           /* e = 6  */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate two working arrays of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp);
        free(newdp);
        return 0;
    }

    dp[1] = 1; /* a(1) = 1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* start with e = 0 contribution: copy dp into newdp */
        for (size_t i = 0; i < sz; ++i) newdp[i] = dp[i];

        idx_t pow = k;
        int e = 1;
        while (e <= 6 && pow <= n) {
            seq_t coeff = COEFF[e];
            for (idx_t m = pow; m <= n; m += pow) {
                newdp[m] += coeff * dp[m / pow];
            }
            if (pow > n / k) break;
            pow *= k;
            ++e;
        }

        /* swap dp and newdp for the next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t ans = dp[n];

    free(dp);
    free(newdp);
    return ans;
}
