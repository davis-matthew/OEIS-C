/*
 * A295843  Number of nX4 0..1 arrays with each 1 adjacent to 1 or 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A295843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which every 1 has 1 or 2
 *            king‑move neighboring 1s. The sequence is computed by a dynamic
 *            programming over rows. Each state stores the current row bits
 *            and the partial neighbor counts of the 1‑cells in that row
 *            (counts from already seen rows). States are encod...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Pre‑compute helper tables */
    static uint8_t left_right[16][4];
    static uint8_t neigh_up[16][4];
    static uint8_t decode_counts[81][4];

    for (int bits = 0; bits < 16; ++bits) {
        for (int c = 0; c < 4; ++c) {
            int left  = (c > 0)   && ((bits >> (c-1)) & 1);
            int right = (c < 3)   && ((bits >> (c+1)) & 1);
            left_right[bits][c] = left + right;
            neigh_up[bits][c]   = left + ((bits >> c) & 1) + right;
        }
    }
    for (int code = 0; code < 81; ++code) {
        int tmp = code;
        for (int c = 0; c < 4; ++c) {
            decode_counts[code][c] = tmp % 3;
            tmp /= 3;
        }
    }

    const int STATE_BITS  = 16;
    const int STATE_COUNT = 16 * 81;   // 1296

    seq_t dp[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];
    /* initialise to zero */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* Initialise states for the first row */
    for (int bits = 0; bits < 16; ++bits) {
        uint8_t counts[4];
        for (int c = 0; c < 4; ++c) {
            if ((bits >> c) & 1) {
                counts[c] = left_right[bits][c];   /* partial from same row */
            } else {
                counts[c] = 0;     /* irrelevant for 0 cells */
            }
        }
        int code = counts[0] + counts[1]*3 + counts[2]*9 + counts[3]*27;
        int key  = bits + STATE_BITS * code;
        dp[key] = (seq_t)1;
    }

    /* Process remaining rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int old_key = 0; old_key < STATE_COUNT; ++old_key) {
            seq_t ways = dp[old_key];
            if (ways == 0) continue;

            int bitsPrev = old_key % STATE_BITS;
            int codePrev = old_key / STATE_BITS;
            uint8_t countsPrev[4];
            for (int c = 0; c < 4; ++c) countsPrev[c] = decode_counts[codePrev][c];

            for (int bitsNext = 0; bitsNext < 16; ++bitsNext) {
                int ok = 1;

                /* Finalise previous row */
                for (int c = 0; c < 4; ++c) {
                    if ((bitsPrev >> c) & 1) {
                        int cnt_final = countsPrev[c] + neigh_up[bitsNext][c];
                        if (cnt_final < 1 || cnt_final > 2) { ok = 0; break; }
                    }
                }
                if (!ok) continue;

                /* Build next state's partial counts */
                uint8_t countsNext[4];
                for (int c = 0; c < 4; ++c) {
                    int partial = left_right[bitsNext][c] + neigh_up[bitsPrev][c];
                    if ((bitsNext >> c) & 1) {
                        if (partial > 2) { ok = 0; break; }
                        countsNext[c] = partial;
                    } else {
                        countsNext[c] = 0;   /* irrelevant */
                    }
                }
                if (!ok) continue;

                int codeNext = countsNext[0] + countsNext[1]*3 + countsNext[2]*9 + countsNext[3]*27;
                int new_key = bitsNext + STATE_BITS * codeNext;
                dp_next[new_key] = dp_next[new_key] + ways;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* Final row validation */
    seq_t result = 0;
    for (int key = 0; key < STATE_COUNT; ++key) {
        seq_t ways = dp[key];
        if (ways == 0) continue;

        int bitsLast = key % STATE_BITS;
        int codeLast = key / STATE_BITS;
        uint8_t countsLast[4];
        for (int c = 0; c < 4; ++c) countsLast[c] = decode_counts[codeLast][c];

        int ok = 1;
        for (int c = 0; c < 4; ++c) {
            if ((bitsLast >> c) & 1) {
                int cnt = countsLast[c];
                if (cnt < 1 || cnt > 2) { ok = 0; break; }
            }
        }
        if (ok) result = result + ways;
    }
    return result;
}
