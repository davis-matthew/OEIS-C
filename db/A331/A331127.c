/*
 * A331127  Number of n-regular hypergraphs on 5 labeled vertices.
 * https://oeis.org/A331127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   a(n) = number of n‑regular hypergraphs on 5 labeled vertices
 *            (count of subsets of the 31 non‑empty subsets of a 5‑vertex set
 *            such that each vertex appears in exactly n chosen subsets)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Pre‑computed constants for base‑17 indexing */
    static const idx_t strides[5] = {1, 17, 289, 4913, 83521};
    /* 17⁵ = 1 419 857, 17⁵ – 1 = 1 419 856 */
    const idx_t MAX_INDEX = 1419856;
    const size_t SIZE = 1419857;          /* number of possible degree states */
    const seq_t SUM_STRIDES = (seq_t)88741; /* 1+17+289+4913+83521 */

    /* Static DP array, computed once */
    static seq_t *dp = NULL;
    static bool initialized = false;

    if (!initialized) {
        /* Allocate and zero the DP array */
#if defined(__cplusplus)
        dp = new seq_t[SIZE]();          /* value‑initialize to zeros */
#else
        dp = calloc(SIZE, sizeof(seq_t));
#endif
        dp[0] = 1;                       /* empty hypergraph has 0 degree */

        /* Enumerate all non‑empty hyperedges (31 in total, masks 1..31) */
        for (int mask = 1; mask <= 31; ++mask) {
            /* Compute the offset for this hyperedge's incidence pattern */
            idx_t offset = 0;
            if (mask & 1)  offset += strides[0];
            if (mask & 2)  offset += strides[1];
            if (mask & 4)  offset += strides[2];
            if (mask & 8)  offset += strides[3];
            if (mask & 16) offset += strides[4];

            /* Update the DP array in reverse to avoid reusing the same edge */
            for (idx_t idx = MAX_INDEX - offset; ; --idx) {
                dp[idx + offset] += dp[idx];
                if (idx == 0) break;
            }
        }
        initialized = true;
    }

    /* Values for n outside 0..16 are zero. */
    if ((seq_t)n > 16)
        return (seq_t)0;

    /* Index for the state (n, n, n, n, n) is n * SUM_STRIDES */
    size_t state_idx = (size_t)((seq_t)n * SUM_STRIDES);
    return dp[state_idx];
}
