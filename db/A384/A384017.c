/*
 * A384017  a(n) = [x^n] Product_{k=0..n} (1 + k*x)^5.
 * https://oeis.org/A384017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=0}^{n} (1 + k*x)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation builds the polynomial iteratively.
    For each factor (1 + kx)^5 it convolves the current
    coefficient array with the 6‑term polynomial
        1 + 5k x + 10 k^2 x^2 + 10 k^3 x^3 + 5 k^4 x^4 + k^5 x^5
    This uses two arrays of length n+1, leading to O(n^2) time
    but only 5*n^2 multiplications in practice – fast enough for n ≤ 10000.
*/

#include <stdlib.h>
#include <string.h>

static const seq_t BINOM5[6] = {1, 5, 10, 10, 5, 1};

seq_t a(idx_t n)
{
    /* Allocate coefficient arrays, zero‑initialized. */
    seq_t *dp   = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp);
        free(newdp);
        return 0;            /* allocation failure guard */
    }

    dp[0] = 1;                /* start with polynomial = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute coefficients of (1 + kx)^5 : 1 + 5k x + … + k^5 x^5 */
        seq_t coeff[6];
        seq_t powk = 1;          /* k^0 */
        coeff[0] = 1;
        for (int j = 1; j <= 5; ++j) {
            powk *= (seq_t)k;    /* k^j */
            coeff[j] = BINOM5[j] * powk;
        }

        /* Clear newdp array for convolution result. */
        memset(newdp, 0, (n + 1) * sizeof(seq_t));

        /* Convolve dp with the 6‑term polynomial, truncating at degree n. */
        for (idx_t i = 0; i <= n; ++i) {
            if (dp[i] == 0) continue;
            seq_t dpi = dp[i];
            for (int j = 0; j <= 5 && i + j <= n; ++j)
                newdp[i + j] += dpi * coeff[j];
        }

        /* Swap dp and newdp for next iteration. */
        seq_t *tmp = dp;
        dp       = newdp;
        newdp    = tmp;
    }

    seq_t ans = dp[n];           /* coefficient of x^n */

    free(dp);
    free(newdp);

    return ans;
}
