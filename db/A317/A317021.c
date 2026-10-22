/*
 * A317021  Expansion of Product_{k>=1} 1/(1 -
 *          x^k)^((3*k-1)*binomial(k+2,3)/2).
 * https://oeis.org/A317021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0; for n > 0: a(n) = (1/n) * Σ_{j=1}^{n} (
 *            Σ_{d|j} d * A002419(d) ) * a(n-j) where A002419(k) = ((3*k-1) *
 *            binom(k+2,3)) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* allocate arrays up to n (inclusive) */
    size_t sz = (size_t)(n + 1);
    seq_t *bket = (seq_t *)calloc(sz, sizeof(seq_t));   /* A002419(k) */
    seq_t *g    = (seq_t *)calloc(sz, sizeof(seq_t));   /* Σ_{d|j} d*b_d  */
    seq_t *arr  = (seq_t *)calloc(sz, sizeof(seq_t));   /* result array   */

    if (!bket || !g || !arr) {
        free(bket); free(g); free(arr);
        return (seq_t)0; /* allocation failure */
    }

    /* compute A002419(k) for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binom(k + 2, (idx_t)3);                 /* binom(k+2,3)   */
        seq_t prod = ((seq_t)(3 * (idx_t)k - 1)) * bin;      /* (3k-1)*bin     */
        bket[k] = prod / (seq_t)2;                          /* divide by 2     */
    }

    /* accumulate g[j] = Σ_{d|j} d * bket[d] */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t term = bket[d] * (seq_t)d;
        for (idx_t m = d; m <= n; m += d)
            g[m] += term;
    }

    /* a(0) = 1 */
    arr[0] = (seq_t)1;

    /* compute a(n) via recurrence */
    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= cur; ++j)
            sum += g[j] * arr[cur - j];
        arr[cur] = sum / (seq_t)cur; /* exact division */
    }

    seq_t result = arr[n];

    free(bket); free(g); free(arr);
    return result;
}
