/*
 * A308768  Sum of the smallest parts of the partitions of n into 4 squarefree
 *          parts.
 * https://oeis.org/A308768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/4)} Sum_{j=k..floor((n-k)/3)}
 *            Sum_{i=j..floor((n-j-k)/2)} mu(k)^2 * mu(j)^2 * mu(i)^2 *
 *            mu(n-i-j-k)^2 * k (where mu is the Möbius function; mu(m)^2 = 1
 *            if m is squarefree, 0 otherwise). The function below uses
 *            dynamic programming to compute the sum of the smallest parts
 *            o...
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Convert n to a size_t for array indexing. */
    size_t N = (size_t)n;
    size_t sz = N + 1;                /* number of sum values 0..N          */
    size_t total = 5 * sz;             /* 0..4 parts, each with sz sums     */

    /* Allocate and zero-initialise DP tables. */
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *w  = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp || !w) {                   /* allocation failure – return 0       */
        return 0;
    }

    /* Base case: zero parts summing to 0. */
    dp[0] = 1;

    /* Process each squarefree part size x. */
    for (idx_t x = 1; x <= n; ++x) {
        if (!is_squarefree((seq_t)x)) continue;    /* skip non‑squarefree x */

        size_t sx = (size_t)x;                    /* x for array index      */
        for (idx_t p = 1; p <= 4; ++p) {          /* number of parts used   */
            size_t off_p   = p * sz;
            size_t off_pp  = (p - 1) * sz;
            for (size_t s = sx; s < sz; ++s) {    /* sum value              */
                size_t idx     = off_p + s;
                size_t idx_prev = off_pp + (s - sx);

                seq_t prev_dp = dp[idx_prev];
                if (!prev_dp) continue;           /* nothing to add          */

                dp[idx] += prev_dp;
                if (p == 1) {
                    /* First part added: the smallest part is x itself. */
                    w[idx] += (seq_t)x * prev_dp;
                } else {
                    /* Smaller parts already determined in dp[p-1][s-x]. */
                    w[idx] += w[idx_prev];
                }
            }
        }
    }

    seq_t result = w[4 * sz + N];   /* smallest‑part sum for 4 parts. */

    free(dp);
    free(w);
    return result;
}
