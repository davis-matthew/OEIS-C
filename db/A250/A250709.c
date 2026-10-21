/*
 * A250709  Number of (2+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250709
 *  a(n) = number of 3×(n+1) arrays over {0,1,2,3} with
 *         non‑decreasing (x(i,j)+x(i,j‑1)) in the i direction and
 *         non‑decreasing min(x(i,j),x(i‑1,j)) in the j direction.
 *  Computed by a DP over column states.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* All possible column states (3 digits 0..3 → 4³ = 64).  */
    static unsigned char s0[64], s1[64], s2[64];
    /* For each previous state, bitmask of allowed next states. */
    static uint64_t trans[64];
    static int initialized = 0;

    if (!initialized) {
        /* Decode each state into its three digits. */
        for (int idx = 0; idx < 64; ++idx) {
            int t = idx;
            s0[idx] = t & 3;          /* least significant digit */
            t >>= 2;
            s1[idx] = t & 3;
            t >>= 2;
            s2[idx] = t & 3;
        }
        /* Build transitions. */
        for (int p = 0; p < 64; ++p) {
            uint64_t mask = 0;
            int p0 = s0[p], p1 = s1[p], p2 = s2[p];
            int prev_t1 = (p1 < p0) ? p1 : p0;      /* min(p1,p0) */
            int prev_t2 = (p2 < p1) ? p2 : p1;      /* min(p2,p1) */
            for (int c = 0; c < 64; ++c) {
                int c0 = s0[c], c1 = s1[c], c2 = s2[c];
                /* s0 <= s1 <= s2 condition */
                if (c0 + p0 > c1 + p1) continue;
                if (c1 + p1 > c2 + p2) continue;
                /* min condition for j‑direction */
                int curr_t1 = (c1 < c0) ? c1 : c0;
                int curr_t2 = (c2 < c1) ? c2 : c1;
                if (curr_t1 < prev_t1) continue;
                if (curr_t2 < prev_t2) continue;
                mask |= ((uint64_t)1 << c);
            }
            trans[p] = mask;
        }
        initialized = 1;
    }

    /* DP arrays holding counts for each column state. */
    static seq_t prev[64], cur[64];

    /* Initial column (j = 0): every state is allowed. */
    for (int i = 0; i < 64; ++i) prev[i] = (seq_t)1;

    /* Process columns j = 1 … n (i.e., n transitions). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) cur[i] = (seq_t)0;
        for (int p = 0; p < 64; ++p) {
            if (prev[p] == (seq_t)0) continue;
            uint64_t mask = trans[p];
            for (int c = 0; c < 64; ++c) {
                if (mask & ((uint64_t)1 << c))
                    cur[c] += prev[p];
            }
        }
        /* Prepare for next step. */
        for (int i = 0; i < 64; ++i) prev[i] = cur[i];
    }

    /* Sum all final states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += prev[i];
    return total;
}
