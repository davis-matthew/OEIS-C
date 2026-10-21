/*
 * A258524  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two maximums of the central row and central column plus the
 *          two minimums of the diagonal and antidiagonal nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A258524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP algorithm counting (n+2)×4 binary arrays whose every 3×3 subblock
 * sum (max of central row + max of central column + min of diagonal
 * + min of anti‑diagonal) is non‑decreasing horizontally, vertically
 * and NE‑to‑SW antidiagonally.
 *
 * The DP keeps as a state the last two rows of the matrix and the sums
 * of the previous row of 3×3 subblocks.  The transition adds a new row,
 * computes the new subblock sums, verifies all monotonicity constraints,
 * and updates the state.  The implementation runs in O(n · STATE) time
 * with STATE = 6400 and fits easily in memory.  It works for any
 * integer type seq_t supplied by the build system.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* precompute the 'sum' for a 3×3 block with given three rows
 * (each row is a 4‑bit value), for the two possible column starts
 * j = 0 and 1. */
static unsigned char S[16][16][16][2];
static bool S_initialized = false;

static void init_S(void)
{
    if (S_initialized) return;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
                for (int j = 0; j < 2; ++j) {
                    /* central row maximum */
                    unsigned char max_cr = ((r1 >> j) & 1) ||
                                          ((r1 >> (j + 1)) & 1) ||
                                          ((r1 >> (j + 2)) & 1);
                    /* central column maximum */
                    unsigned char max_cc = ((r0 >> (j + 1)) & 1) ||
                                           ((r1 >> (j + 1)) & 1) ||
                                           ((r2 >> (j + 1)) & 1);
                    /* diagonal minimum (AND of the three bits) */
                    unsigned char diag_min = ((r0 >> j) & 1) &
                                            ((r1 >> (j + 1)) & 1) &
                                            ((r2 >> (j + 2)) & 1);
                    /* anti‑diagonal minimum */
                    unsigned char anti_min = ((r0 >> (j + 2)) & 1) &
                                             ((r1 >> (j + 1)) & 1) &
                                             ((r2 >> j) & 1);
                    S[r0][r1][r2][j] = max_cr + max_cc + diag_min + anti_min;
                }
    S_initialized = true;
}

/* State representation:
 *   pair of last two rows (each 4 bits)
 *   sums of the previous subblock row (two values 0…4)
 * We encode the state as a single integer:
 *   idx = ((r_prev << 4) | r_cur) * 25 + (s0 * 5 + s1)
 * where s0,s1 ∈ {0,…,4} and r_prev,r_cur ∈ {0,…,15}.
 */
#define NUM_PAIRS   256          /* 16 × 16 */
#define S_MAX       5            /* possible sum values 0…4 */
#define STATE_SIZE  (NUM_PAIRS * S_MAX * S_MAX)   /* 6400 */

static unsigned short transitions[STATE_SIZE][16];
static unsigned char trans_count[STATE_SIZE];
static bool trans_initialized = false;

static void init_transitions(void)
{
    if (trans_initialized) return;
    init_S();
    for (int idx = 0; idx < STATE_SIZE; ++idx) {
        int pair_idx = idx / (S_MAX * S_MAX);
        int sidx     = idx % (S_MAX * S_MAX);
        int r_prev   = pair_idx >> 4;
        int r_cur    = pair_idx & 0xF;
        int s0       = sidx / S_MAX;
        int s1       = sidx % S_MAX;

        unsigned char count = 0;
        for (int r_next = 0; r_next < 16; ++r_next) {
            unsigned char ns0 = S[r_prev][r_cur][r_next][0];
            unsigned char ns1 = S[r_prev][r_cur][r_next][1];
            /* constraints for new subblock row */
            if (ns0 <= ns1 &&             /* horizontal */
                s0 <= ns0 && s1 <= ns1 && /* vertical */
                s1 <= ns0) {              /* anti‑diagonal */
                int new_pair_idx = (r_cur << 4) | r_next;
                int new_sidx     = ns0 * S_MAX + ns1;
                int new_full     = new_pair_idx * (S_MAX * S_MAX) + new_sidx;
                transitions[idx][count++] = (unsigned short)new_full;
            }
        }
        trans_count[idx] = count;
    }
    trans_initialized = true;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    init_transitions();

    static seq_t cur[STATE_SIZE];
    static seq_t nxt[STATE_SIZE];

    /* clear DP tables */
    for (size_t i = 0; i < STATE_SIZE; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }

    /* initial states from the first three rows */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2) {
                unsigned char s0 = S[r0][r1][r2][0];
                unsigned char s1 = S[r0][r1][r2][1];
                if (s0 <= s1) {
                    int pair_idx = (r1 << 4) | r2;   /* store rows r1 and r2 */
                    int sidx     = s0 * S_MAX + s1;
                    int idx      = pair_idx * (S_MAX * S_MAX) + sidx;
                    cur[idx] += 1;
                }
            }

    if (n == 1) {                 /* only horizontal constraint applies */
        seq_t ans = 0;
        for (size_t i = 0; i < STATE_SIZE; ++i) ans += cur[i];
        return ans;
    }

    /* add further rows */
    for (idx_t step = 1; step < n; ++step) {
        for (size_t i = 0; i < STATE_SIZE; ++i) nxt[i] = 0;
        for (int idx = 0; idx < STATE_SIZE; ++idx) {
            seq_t cnt = cur[idx];
            if (cnt == 0) continue;
            unsigned char tcnt = trans_count[idx];
            for (unsigned char t = 0; t < tcnt; ++t) {
                unsigned short nidx = transitions[idx][t];
                nxt[nidx] += cnt;
            }
        }
        /* swap tables */
        for (size_t i = 0; i < STATE_SIZE; ++i) cur[i] = nxt[i];
    }

    seq_t ans = 0;
    for (size_t i = 0; i < STATE_SIZE; ++i) ans += cur[i];
    return ans;
}
