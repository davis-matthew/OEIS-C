/*
 * A371007  Expansion of e.g.f. (1/x) * Series_Reversion( 2*x/(3*exp(2*x) - 1)
 *          ).
 * https://oeis.org/A371007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = n! * Σ_{k=0..n} 3^k * 2^(n-k) * Stirling2(n,k)/(n-k+1)!
 *            * where Stirling2(n,k) is the Stirling number of the second kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   compute_stirling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_stirling.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1; /* a(0)=1 */
    }

    /* -- 1. Compute n!  */
    seq_t fac = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fac = fac * (seq_t)i;
    }

    /* -- 2. Compute Stirling numbers S(n,k) for k=0..n  */
    seq_t *S = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!S) {
        return 0; /* allocation failure, should not happen in tests */
    }
    compute_stirling(n, S);

    /* -- 3. Pre‑compute powers of 2^n  */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2_n = pow2_n * 2;
    }
    /* pow2 initially equals 2^(n-1) for the first term (k=1) */
    seq_t pow2 = pow2_n / 2;
    /* -- 4. Initialize power of 3 and the running ratio */
    seq_t pow3 = 3;               /* 3^1  */
    seq_t ratio = 1;              /* fac/(n-1+1)! = fac/n! = 1 for k=1 */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* term = ratio * S(n,k) * pow3 * pow2 */
        seq_t term = ratio;
        term = term * S[k];
        term = term * pow3;
        term = term * pow2;
        sum = sum + term;

        if (k == n) {
            break; /* no more terms */
        }

        /* Prepare for next k+1 */
        ratio = ratio * ((seq_t)(n - k + 1)); /* multiply by (n-k+1) */
        pow3 = pow3 * 3;                      /* 3^(k+1) */
        pow2 = pow2 / 2;                      /* 2^(n-(k+1)) */
    }

    free(S);
    return sum;
}
