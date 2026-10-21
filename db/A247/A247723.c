/*
 * A247723  Number of length n+3 0..5 arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over {0..5} with no disjoint
 *            pairs in any consecutive four terms having the same sum
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   triple_to_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a linear‑time DP over triples.
   The DP state is the last three elements of the array; an
   allowed transition from (x,y,z) to (y,z,w) exists iff
   in the block (x,y,z,w) all three disjoint‑pair sums differ. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/triple_to_index.h"

static int nextState[216][6];
static bool initialized = false;

static inline void init_transition_table(void) {
    for (int idx = 0; idx < 216; ++idx) {
        int x = idx / 36;
        int y = (idx % 36) / 6;
        int z = idx % 6;
        for (int w = 0; w < 6; ++w) {
            int s1 = x + y;
            int s2 = z + w;
            int s3 = x + z;
            int s4 = y + w;
            int s5 = x + w;
            int s6 = y + z;
            if (s1 == s2 || s3 == s4 || s5 == s6) {
                nextState[idx][w] = -1;   /* invalid transition */
            } else {
                nextState[idx][w] = triple_to_index(y, z, w);
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_transition_table();
        initialized = true;
    }

    /* DP arrays */
    seq_t cur[216];
    seq_t nxt[216];

    /* initial length 3: all triples are allowed */
    for (int i = 0; i < 216; ++i) cur[i] = 1;

    /* Perform n steps to reach length n+3 */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 216; ++i) nxt[i] = 0;
        for (int idx = 0; idx < 216; ++idx) {
            seq_t value = cur[idx];
            if (!value) continue;
            for (int w = 0; w < 6; ++w) {
                int nxt_idx = nextState[idx][w];
                if (nxt_idx >= 0) {
                    nxt[nxt_idx] += value;
                }
            }
        }
        /* copy nxt to cur */
        for (int i = 0; i < 216; ++i) cur[i] = nxt[i];
    }

    /* Sum over all ending triples */
    seq_t total = 0;
    for (int i = 0; i < 216; ++i) total += cur[i];
    return total;
}
