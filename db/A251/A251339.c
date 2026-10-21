/*
 * A251339  Number of (n+1)X(4+1) 0..3 arrays with every 2X2 subblock summing
 *          to a nonzero multiple of 3.
 * https://oeis.org/A251339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: number of (n+1)×5 0..3 arrays where every 2×2
   subblock sums to a non‑zero multiple of 3.
   The algorithm represents each row as a 5‑digit base‑4 state
   (total 4^5 = 1024 states).  Two states are compatible if
   for each of the four 2×2 blocks formed by adjacent rows the
   four entries sum to a multiple of 3 and are not zero.
   DP: start with any state for the first row; for each of the
   remaining n rows propagate counts through the compatibility
   graph.  The total number of arrays is the sum of the counts
   after n transitions.  This is an O(n·E) algorithm where E ≈ 12 000
   is the number of edges in the compatibility graph, fast enough
   for n ≤ 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- Build the compatibility graph once ----------------- */
    static bool initialized = false;
    static unsigned int state_digits[1024][5];
    static unsigned short *adj = NULL;      /* flattened adjacency list */
    static int *adj_start = NULL;           /* start index for each node */
    static int *adj_deg   = NULL;           /* degree of each node */

    if (!initialized) {
        /* Pre‑compute the 5 digits of each state (base 4). */
        for (int s = 0; s < 1024; ++s) {
            int x = s;
            for (int r = 0; r < 5; ++r) {
                state_digits[s][r] = x & 3;
                x >>= 2;
            }
        }

        /* First pass: compute degrees and total number of edges. */
        int total_edges = 0;
        adj_deg = (int *)malloc(1024 * sizeof(int));
        for (int s = 0; s < 1024; ++s) {
            int d = 0;
            for (int t = 0; t < 1024; ++t) {
                int ok = 1;
                for (int r = 0; r < 4; ++r) {
                    int sum = state_digits[s][r] + state_digits[s][r + 1]
                            + state_digits[t][r] + state_digits[t][r + 1];
                    if (sum == 0 || (sum % 3) != 0) { ok = 0; break; }
                }
                if (ok) ++d;
            }
            adj_deg[s] = d;
            total_edges += d;
        }

        /* Allocate flattened adjacency list. */
        adj = (unsigned short *)malloc(total_edges * sizeof(unsigned short));

        /* Compute start indices for each node. */
        adj_start = (int *)malloc(1025 * sizeof(int));
        adj_start[0] = 0;
        for (int s = 0; s < 1024; ++s)
            adj_start[s + 1] = adj_start[s] + adj_deg[s];

        /* Second pass: fill adjacency list. */
        int pos = 0;
        for (int s = 0; s < 1024; ++s) {
            for (int t = 0; t < 1024; ++t) {
                int ok = 1;
                for (int r = 0; r < 4; ++r) {
                    int sum = state_digits[s][r] + state_digits[s][r + 1]
                            + state_digits[t][r] + state_digits[t][r + 1];
                    if (sum == 0 || (sum % 3) != 0) { ok = 0; break; }
                }
                if (ok) {
                    adj[pos++] = (unsigned short)t;
                }
            }
        }
        initialized = true;
    }

    /* ---------- DP over the rows ----------------------------------- */
    static seq_t dp[1024], dp_next[1024];
    for (int i = 0; i < 1024; ++i)
        dp[i] = (seq_t)1;                 /* first row can be any state */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 1024; ++i)
            dp_next[i] = 0;

        for (int s = 0; s < 1024; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int deg     = adj_deg[s];
            int offset  = adj_start[s];
            for (int k = 0; k < deg; ++k) {
                unsigned short t = adj[offset + k];
                dp_next[t] += val;
            }
        }

        for (int i = 0; i < 1024; ++i)
            dp[i] = dp_next[i];
    }

    /* ---------- Sum all terminal states ----------------------------- */
    seq_t result = 0;
    for (int i = 0; i < 1024; ++i)
        result += dp[i];

    return result;
}
