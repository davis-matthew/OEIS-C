/*
 * A299404  a(n) = 1 + Sum_{m >= 1} (m + 1)^n/2^(m - 1).
 * https://oeis.org/A299404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{m >= 1} (m + 1)^n / 2^(m - 1) (OEIS A299404)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    idx_t N = n;                     /* index is 64‑bit unsigned */
    /* Allocate temporary arrays */
    seq_t *fact  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *stir  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *S     = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!fact || !stir || !S) {
        /* Allocation failure – return 0 as a graceful fallback */
        return (seq_t)0;
    }

    /* 1. Pre‑compute factorials fact[i] = i!   */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* 2. Compute Stirling numbers of the second kind row by row
       and simultaneously compute S[j] = 2 * Σ_{i=0}^j S2(j,i) * fact[i]   */
    for (idx_t i = 0; i <= N; ++i) {
        stir[i] = (seq_t)0;
    }
    stir[0] = (seq_t)1;   /* S2(0,0) = 1 */

    for (idx_t j = 0; j <= N; ++j) {
        /* sum = Σ_{i=0}^{j} stir[i] * fact[i]   */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= j; ++i) {
            sum += stir[i] * fact[i];
        }
        S[j] = sum * (seq_t)2;    /* b_j = 2 * sum; b_j is the integer b_j in the derivation */

        /* Prepare stir for next j+1 */
        if (j < N) {
            for (idx_t i = j + 1; i >= 1; --i) {
                stir[i] = stir[i] * (seq_t)i + stir[i - 1];
            }
            stir[0] = (seq_t)0;
        }
    }

    /* 3. Accumulate the final sum: a(n) = 1 + Σ_{j=0}^n C(n,j) * 2^{n-j} * S[j]   */
    seq_t result = (seq_t)1;      /* the leading +1 */
    seq_t bin    = (seq_t)1;      /* binom coefficient C(n,0) */
    seq_t power  = pow2(N);       /* 2^N  */
    for (idx_t j = 0; j <= N; ++j) {
        result += bin * power * S[j];
        if (j == N) break;
        bin    = bin * (N - j) / (j + 1); /* update C(n,j+1) */
        power >>= 1;                      /* 2^{n-(j+1)} */
    }

    /* Clean up */
    free(fact);
    free(stir);
    free(S);

    return result;
}
