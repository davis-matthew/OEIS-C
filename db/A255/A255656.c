/*
 * A255656  Number of length n+4 0..3 arrays with at most two downsteps in
 *          every 4 consecutive neighbor pairs.
 * https://oeis.org/A255656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255656: number of length n+4 0..3 arrays with at most two downsteps
 * in any 5 consecutive elements.  Computed by dynamic programming on the last
 * four values; no hard‑coded table of terms.  The DP state is the 4‑tuple
 * (a_{i-4},a_{i-3},a_{i-2},a_{i-1}) of the last four values; transitions
 * add a new value a_i only if the four adjacent pairs among the five
 * consecutive elements contain at most two downsteps. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The array length is n+4.  For n>=1 this is at least 5. */
    idx_t m = n + (idx_t)4;
    if (m < (idx_t)5) {
        /* Undefined for offset less than 1; return 0. */
        return (seq_t)0;
    }

    /* Precomputed table: down[x][y] == 1 if y<x otherwise 0. */
    static const unsigned char down[4][4] = {
        {0,0,0,0},
        {1,0,0,0},
        {1,1,0,0},
        {1,1,1,0}
    };

    /* DP arrays for current and previous layers.  Size 256 (=4^4). */
    seq_t prev[256], cur[256];
    int i;

    /* Initial state: all length‑4 sequences are allowed.  Each state
     * contributes one sequence. */
    for (i = 0; i < 256; ++i) {
        prev[i] = (seq_t)1;
    }

    /* Iterate through lengths 5 .. m.  After the loop, prev holds the counts
     * for length m. */
    idx_t len;
    for (len = (idx_t)5; len <= m; ++len) {
        /* Clear current layer. */
        for (i = 0; i < 256; ++i) {
            cur[i] = (seq_t)0;
        }

        /* For each previous state, try all 4 possible new values. */
        for (i = 0; i < 256; ++i) {
            seq_t add = prev[i];
            if (!add) {
                continue;
            }
            unsigned v1 = (unsigned)(i >> 6);               /* bits 6–7 */
            unsigned v2 = (unsigned)((i >> 4) & 3);        /* bits 4–5 */
            unsigned v3 = (unsigned)((i >> 2) & 3);        /* bits 2–3 */
            unsigned v4 = (unsigned)(i & 3);               /* bits 0–1 */

            /* Try each next value v5. */
            unsigned ns;
            for (unsigned v5 = 0; v5 < 4; ++v5) {
                unsigned down_sum =
                    down[v1][v2] + down[v2][v3] + down[v3][v4] + down[v4][v5];
                if (down_sum <= 2u) {
                    /* Shift window left by one, append v5. */
                    ns = ((v2 << 6) | (v3 << 4) | (v4 << 2) | v5);
                    cur[ns] += add;
                }
            }
        }

        /* Prepare for next iteration: swap layers. */
        for (i = 0; i < 256; ++i) {
            prev[i] = cur[i];
        }
    }

    /* Total number of sequences of length m is the sum over all final states. */
    seq_t total = (seq_t)0;
    for (i = 0; i < 256; ++i) {
        total += prev[i];
    }

    return total;
}
