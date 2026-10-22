/*
 * A383862  a(n) = [x^n] Product_{k=0..n} 1/(1 - k*x)^3.
 * https://oeis.org/A383862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i,j,k≥0, i+j+k=n} Stirling2(i+n,n) *
 *            Stirling2(j+n,n) * Stirling2(k+n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, factorial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculated by dynamic programming on Stirling numbers of the second kind
 * and a direct double‑loop convolution.                                            */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binom.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }

    /* size of arrays: n+1 */
    idx_t sz = n + 1;

    /* Allocate temporary arrays */
    seq_t *prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *S    = (seq_t *)calloc(sz, sizeof(seq_t));   /* S[i] = Stirling2(i+n, n),  i = 0..n */

    if (!prev || !cur || !S) {
        /* allocation failure – returning 0 as safe fallback */
        free(prev); free(cur); free(S);
        return (seq_t)0;
    }

    /* Initial row: S(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Build all Stirling numbers S(m,k) with k ≤ n and m ≤ 2n,
       storing the column k = n values needed for the sums. */
    for (idx_t m = 1; m <= 2 * n; ++m) {
        cur[0] = 0;
        idx_t maxk = (m < n) ? m : n;
        for (idx_t k = 1; k <= maxk; ++k) {
            /* cur[k] = k * prev[k] + prev[k-1] */
            seq_t mult = mul_small(prev[k], (unsigned int)k);
            cur[k] = mult + prev[k - 1];
        }
        if (m >= n) {
            S[m - n] = cur[n];
        }
        /* swap prev and cur */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Compute a(n) via double loop over i and j, with k = n - i - j. */
    seq_t result = 0;
    for (idx_t i = 0; i <= n; ++i) {
        for (idx_t j = 0; j <= n - i; ++j) {
            idx_t k = n - i - j;
            seq_t prod = seq_mul(S[i], S[j]);    /* S[i] * S[j] */
            prod = seq_mul(prod, S[k]);           /* * S[k] */
            result += prod;
        }
    }

    /* Clean up */
    free(prev);
    free(cur);
    free(S);

    return result;
}
