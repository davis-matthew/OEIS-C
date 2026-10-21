/*
 * A285174  a(n) is the number of Dyck paths of (2,3)-knight moves of size n.
 * https://oeis.org/A285174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of Dyck paths of (2,3)-knight moves of size n. *
 *            Computation: dynamic programming over horizontal coordinate x
 *            and * current height h. For each step of length 2 or 3 we add or
 *            subtract * the appropriate vertical offset while keeping the
 *            height non‑negative
 * Range:     u8 n <= 19, u16 n <= 30, u32 n <= 51, u64 n <= 91, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, calloc, free, abort */

/* Arguments:
 *   n – non‑negative index
 * Return:
 *   a(n) – number of Dyck paths of (2,3)-knight moves ending at (n,0).   */
seq_t a(idx_t n)
{
    /* Special case n == 0.  The empty path is the only one.          */
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }

    /* The maximum possible height at position n is (3*n)/2.
       Add a small margin to avoid off‑by‑one when indexing arrays. */
    const idx_t maxH = (n * 3) / 2 + 5;

    /* Allocate four rolling arrays of size maxH+1 each.            */
    seq_t *dp[4];
    for (idx_t i = 0; i < 4; ++i) {
#if defined(__cplusplus)
        dp[i] = new seq_t[maxH + 1]();   /* value‑initialise to 0 */
#else
        dp[i] = (seq_t *)calloc((size_t)(maxH + 1), sizeof(seq_t));
        if (!dp[i]) abort();
#endif
    }

    /* Base case: one empty path at (0,0).                         */
    dp[0][0] = (seq_t)1;

    /* Dynamic‑programming loop over horizontal coordinate x.      */
    for (idx_t x = 1; x <= n; ++x) {
        const idx_t cur  = x & 3;           /* x % 4        */
        seq_t *dp_cur = dp[cur];

        /* Zero all entries up to the reachable height.            */
        const idx_t maxH_cur = (x * 3) / 2;
        for (idx_t h = 0; h <= maxH_cur; ++h) dp_cur[h] = 0;

        /* Contributions from steps of length 2, vertical ±3.      */
        if (x >= 2) {
            const idx_t from     = (x - 2) & 3;
            seq_t *dp_prev = dp[from];
            const idx_t maxH_prev = ((x - 2) * 3) / 2;
            for (idx_t h = 0; h <= maxH_prev; ++h) {
                seq_t val = dp_prev[h];
                if (val == 0) continue;
                /* move up by +3 */
                if (h + 3 <= maxH_cur) dp_cur[h + 3] += val;
                /* move down by -3 */
                if (h >= 3)           dp_cur[h - 3] += val;
            }
        }

        /* Contributions from steps of length 3, vertical ±2.      */
        if (x >= 3) {
            const idx_t from     = (x - 3) & 3;
            seq_t *dp_prev = dp[from];
            const idx_t maxH_prev = ((x - 3) * 3) / 2;
            for (idx_t h = 0; h <= maxH_prev; ++h) {
                seq_t val = dp_prev[h];
                if (val == 0) continue;
                /* move up by +2 */
                if (h + 2 <= maxH_cur) dp_cur[h + 2] += val;
                /* move down by -2 */
                if (h >= 2)           dp_cur[h - 2] += val;
            }
        }
    }

    /* Retrieve the answer before freeing memory.                  */
    const seq_t result = dp[n & 3][0];

    /* Free the allocated arrays.                                  */
    for (idx_t i = 0; i < 4; ++i) {
#if defined(__cplusplus)
        delete[] dp[i];
#else
        free(dp[i]);
#endif
    }

    return result;
}
