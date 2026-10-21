/*
 * A248838  Number of length n arrays x(i), i=1..n with x(i) in 0..i and no
 *          value appearing more than 4 times.
 * https://oeis.org/A248838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays x(i), i=1..n with 0 ≤ x(i) ≤ i
 *            * and no value appearing more than 4 times. * Computed by
 *            dynamic programming over the values in descending order. * * The
 *            DP keeps dp[used] = number of ways after processing all higher *
 *            values, where *used* counts how many position...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline seq_t binom4(idx_t n, idx_t k)
{
    seq_t res = 1;
    if (k == 0) return 1;
    if (k > 4 || k > n) return 0;
    seq_t nn = (seq_t)n;
    if (k > 0) res = res * nn / (seq_t)1;   /* C(n,1) */
    if (k > 1) res = res * (nn - 1) / (seq_t)2; /* C(n,2) */
    if (k > 2) res = res * (nn - 2) / (seq_t)3; /* C(n,3) */
    if (k > 3) res = res * (nn - 3) / (seq_t)4; /* C(n,4) */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1: a(1) is first term */

    /* allocate DP arrays */
    seq_t *dp   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return 0;
    }

    dp[0] = 1;          /* no positions used yet */

    for (idx_t v = n; ; --v) {
        /* number of positions with index ≥ v */
        idx_t totalPos = (v == 0) ? n : (n - v + 1);

        idx_t usedLimit = (v == 0) ? n : (n - v);   /* max used so far */

        for (idx_t used = 0; used <= usedLimit; ++used) {
            seq_t ways = dp[used];
            if (!ways) continue;
            idx_t free = totalPos - used;
            if (free == (idx_t)-1) continue;      /* safety */

            idx_t maxk = free < 4 ? free : 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t bin = binom4(free, k);
                next[used + k] = next[used + k] + ways * bin;
            }
        }

        /* prepare for next smaller value */
        seq_t *tmp = dp; dp = next; next = tmp;
        /* zero out next for reuse */
        size_t clearSize = (size_t)(n + 1) * sizeof(seq_t);
        memset(next, 0, clearSize);

        if (v == 0) break;   /* finished all values */
    }

    seq_t result = dp[n];
    free(dp); free(next);
    return result;
}
