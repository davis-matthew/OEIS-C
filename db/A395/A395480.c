/*
 * A395480  Number of n-color compositions of n with parts in standard order.
 * https://oeis.org/A395480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑color compositions of n with parts in
 *            standard order. The function uses dynamic programming that runs
 *            in O(n·k²) time, where k ≈ √(2n). The maximal part value used in
 *            a valid composition of n is at most floor((√(8n+1)-1)/2), so the
 *            DP state space is (n+1)×(k+1)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 96,
 *            big any n
 * Helpers:   max_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/max_part.h"

/*  seq_t and idx_t are supplied by the build system.  */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Determine the maximum part value needed */
    idx_t kmax = max_part(n);
    size_t msize = (size_t)(kmax + 1);          /* number of possible max‑part values */
    size_t cells = ((size_t)(n + 1)) * msize;   /* total number of DP entries */

    /* Allocate DP table and zero‑initialize it */
    seq_t *dp = (seq_t *)calloc(cells, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                      /* allocation failed */

    /* Helper to compute linear index in DP array */
    #define IDX(s, m) (((size_t)(s)) * msize + (size_t)(m))

    dp[IDX(0, 0)] = (seq_t)1;                 /* start state: sum 0, max part 0 */

    /* Main DP: build compositions by adding parts one at a time */
    for (idx_t s = 0; s <= n; ++s) {
        for (idx_t m = 0; m <= kmax; ++m) {
            seq_t val = dp[IDX(s, m)];
            if (!val)
                continue;

            /* Add a new part with value m+1 (first occurrence of a larger part) */
            if (m + 1 <= kmax) {
                idx_t ns = s + m + 1;
                if (ns <= n) {
                    dp[IDX(ns, m + 1)] += val * (seq_t)(m + 1);
                }
            }

            /* Add another copy of an existing part (1 .. m) */
            if (m) {
                for (idx_t v = 1; v <= m; ++v) {
                    idx_t ns = s + v;
                    if (ns > n)
                        break;
                    dp[IDX(ns, m)] += val * (seq_t)v;
                }
            }
        }
    }

    /* Sum all DP states that reach total n with a positive max part */
    seq_t result = (seq_t)0;
    for (idx_t m = 1; m <= kmax; ++m) {
        result += dp[IDX(n, m)];
    }

    free(dp);
    return result;
}
