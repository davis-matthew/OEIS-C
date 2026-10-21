/*
 * A253744  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A253744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 4 arrays of entries 0..2 such that for
 *            every 2×2 subblock the NE–SW antidiagonal difference is
 *            non‑decreasing horizontally and the NW+SE diagonal sum is
 *            non‑decreasing vertically. Computed by dynamic programming over
 *            rows with state (previous row, previous vertical sums)
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 47, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define INVALID_S 255U

/* Pre‑computed tables (size is tiny so static storage is fine). */
static uint8_t row_vals[81][4];
static uint8_t sum0_of_idx[125];
static uint8_t sum1_of_idx[125];
static uint8_t sum2_of_idx[125];
static uint8_t trans_sidx[81][81];
static uint8_t trans_sum0[81][81];
static uint8_t trans_sum1[81][81];
static uint8_t trans_sum2[81][81];

/* Build the tables once. */
static void init(void)
{
    /* Enumerate all rows (4 digits base 3). */
    for (int id = 0; id < 81; ++id) {
        int tmp = id;
        for (int col = 0; col < 4; ++col) {
            row_vals[id][col] = tmp % 3;
            tmp /= 3;
        }
    }

    /* Encode all possible sum vectors (0..4). */
    for (int s = 0; s < 125; ++s) {
        sum0_of_idx[s] = s / 25;
        sum1_of_idx[s] = (s / 5) % 5;
        sum2_of_idx[s] = s % 5;
    }

    /* Compute all valid transitions between consecutive rows. */
    for (int a = 0; a < 81; ++a) {
        for (int b = 0; b < 81; ++b) {
            int diff0 = (int)row_vals[a][1] - (int)row_vals[b][0];
            int diff1 = (int)row_vals[a][2] - (int)row_vals[b][1];
            int diff2 = (int)row_vals[a][3] - (int)row_vals[b][2];
            if (diff0 <= diff1 && diff1 <= diff2) {
                uint8_t sum0 = row_vals[a][0] + row_vals[b][1];
                uint8_t sum1 = row_vals[a][1] + row_vals[b][2];
                uint8_t sum2 = row_vals[a][2] + row_vals[b][3];
                uint8_t sidx = sum0 * 25 + sum1 * 5 + sum2;
                trans_sidx[a][b] = sidx;
                trans_sum0[a][b] = sum0;
                trans_sum1[a][b] = sum1;
                trans_sum2[a][b] = sum2;
            } else {
                trans_sidx[a][b] = INVALID_S;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static int init_done = 0;
    if (!init_done) {
        init();
        init_done = 1;
    }

    /* 81 rows × 125 sum states = 10125 states. */
    const size_t STATES = 81 * 125;
    seq_t *dp_prev = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* Base case: all pairs of first two rows that satisfy horizontal
       constraint. */
    for (int a = 0; a < 81; ++a) {
        for (int b = 0; b < 81; ++b) {
            uint8_t sidx = trans_sidx[a][b];
            if (sidx != INVALID_S) {
                dp_prev[b * 125 + sidx] += (seq_t)1;
            }
        }
    }

    /* For n = 1 we are already done.  Otherwise we need to add (n‑1)
       more rows. */
    if (n > 1) {
        idx_t steps = n - 1;
        for (idx_t step = 0; step < steps; ++step) {
            /* Clear next DP table. */
            memset(dp_next, 0, STATES * sizeof(seq_t));

            for (int a = 0; a < 81; ++a) {
                size_t base_a = a * 125;
                for (int s = 0; s < 125; ++s) {
                    seq_t cnt = dp_prev[base_a + s];
                    if (!cnt)
                        continue;
                    uint8_t s0_prev = sum0_of_idx[s];
                    uint8_t s1_prev = sum1_of_idx[s];
                    uint8_t s2_prev = sum2_of_idx[s];

                    for (int b = 0; b < 81; ++b) {
                        uint8_t sidx2 = trans_sidx[a][b];
                        if (sidx2 == INVALID_S)
                            continue;

                        /* Check vertical monotone condition. */
                        if (trans_sum0[a][b] < s0_prev ||
                            trans_sum1[a][b] < s1_prev ||
                            trans_sum2[a][b] < s2_prev)
                            continue;

                        dp_next[b * 125 + sidx2] += cnt;
                    }
                }
            }
            /* Swap DP tables. */
            seq_t *tmp = dp_prev;
            dp_prev = dp_next;
            dp_next = tmp;
        }
    }

    /* Sum all counts in the final DP table. */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i)
        total += dp_prev[i];

    free(dp_prev);
    free(dp_next);
    return total;
}
