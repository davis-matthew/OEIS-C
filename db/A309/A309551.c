/*
 * A309551  Number of even parts in the partitions of n into 6 parts.
 * https://oeis.org/A309551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of even parts in all partitions of n into
 *            exactly 6 parts
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Allocate two flat arrays: dp[k][s] and ev[k][s], flattened as row‑major. */
    const size_t stride = (size_t)n + 1;          /* columns per part count */
    const size_t cells  = 7 * stride;             /* k = 0..6 */

    /* Use calloc so that all entries are zero‑initialized.  The cast is needed
       for C++ compilers. */
    seq_t *dp = (seq_t *)calloc(cells, sizeof(seq_t));
    seq_t *ev = (seq_t *)calloc(cells, sizeof(seq_t));
    if (!dp || !ev) {
        free(dp);
        free(ev);
        return (seq_t)0;
    }

    /* Base case: one way to have zero parts summing to 0. */
    dp[0] = (seq_t)1;

    /* Standard integer‑partition DP that counts partitions with
       an exact number of parts and sums the number of even parts. */
    for (idx_t v = 1; v <= n; ++v) {
        for (idx_t s = v; s <= n; ++s) {
            for (idx_t k = 1; k <= 6; ++k) {
                seq_t add_cnt = dp[(k - 1) * stride + (s - v)];
                if (!add_cnt) continue; /* nothing to add */

                /* Update partition count. */
                dp[k * stride + s] += add_cnt;

                /* Update even‑part count. */
                seq_t add_ev = ev[(k - 1) * stride + (s - v)];
                ev[k * stride + s] += add_ev;
                if ((v & 1) == 0)               /* v is even */
                    ev[k * stride + s] += add_cnt;
            }
        }
    }

    seq_t result = ev[6 * stride + n];

    free(dp);
    free(ev);
    return result;
}
