/*
 * A274837  Number of set partitions of [n] such that the difference between
 *          each element and its block index is a multiple of four.
 * https://oeis.org/A274837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of set partitions of [n] such that for every *
 *            element i and its block index b (counting the blocks by
 *            increasing * minimum element, starting at 1), (i - b) is a
 *            multiple of 4. The * following dynamic programming counts these
 *            partitions. * * For each processed element j (1‑based) we...
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 28, u64 n <= 43, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate dp arrays of size n+1.  For n==0 we still allocate 1. */
    seq_t *dp = (seq_t*)calloc(n + 1, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!dp || !next) return (seq_t)0;  /* should never happen */

    dp[0] = (seq_t)1;  /* empty set has exactly one partition */

    for (idx_t i = 0; i < n; ++i) {        /* i -> number of elements already processed */
        idx_t j = i + 1;                   /* next element (1‑based) */
        int d = (int)(j & 3);              /* j mod 4 */

        /* zero next for indices that will be written (0..i+1) */
        memset(next, 0, (i + 2) * sizeof(seq_t));

        for (idx_t k = 0; k <= i; ++k) {
            seq_t val = dp[k];
            if (!val) continue;

            /* cnt_d(k) */
            seq_t m = (seq_t)(k >> 2);      /* floor(k/4) */
            int r = k & 3;                  /* k mod 4 */
            seq_t cnt = (d == 0) ? m : m + ((d <= r) ? 1 : 0);

            if (cnt) { /* add to existing block of residue d */
                next[k] += val * cnt;
            }

            /* start a new block if its residue matches d */
            if (((k + 1) & 3) == d) {
                next[k + 1] += val;
            }
        }

        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Sum over all possible numbers of started blocks */
    seq_t ans = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        ans += dp[k];
    }

    /* free temporary storage */
    free(dp);
    free(next);

    return ans;
}
