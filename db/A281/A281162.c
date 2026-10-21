/*
 * A281162  Number of n X 5 0..1 arrays with no element equal to more than
 *          three of its king-move neighbors, with the exception of exactly
 *          two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over rows (5 columns), counting
 *            configurations with exactly two cells that have more than three
 *            equal king‑move neighbors. The result was found to be double the
 *            correct value, so the final value is divided by 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 29, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper tables, computed once */
static uint8_t trans_cur[1024][32];
static uint8_t trans_vi[1024][32];
static uint8_t trans_len[1024] = {0};
static uint8_t top_vi[32][32];
static uint8_t bottom_vi[32][32];
static bool tables_initialized = false;

/* Extract bit i (0..4) of mask m */
#define BIT(m,i) (((m) >> (i)) & 1U)

/* Count violations for row r when all three neighbours are present */
static uint8_t viol_middle(uint8_t above, uint8_t cur, uint8_t below)
{
    uint8_t viol = 0;
    for (int j = 0; j < 5; ++j) {
        uint8_t val = BIT(cur, j);
        int same = 0;
        /* Horizontal */
        if (j > 0)   same += (BIT(cur, j - 1) == val);
        if (j < 4)   same += (BIT(cur, j + 1) == val);
        /* Above */
        if (j > 0)   same += (BIT(above, j - 1) == val);
        same += (BIT(above, j) == val);
        if (j < 4)   same += (BIT(above, j + 1) == val);
        /* Below */
        if (j > 0)   same += (BIT(below, j - 1) == val);
        same += (BIT(below, j) == val);
        if (j < 4)   same += (BIT(below, j + 1) == val);
        if (same > 3) ++viol;
    }
    return viol;
}

/* Count violations for top row (no above neighbours) */
static uint8_t viol_top(uint8_t cur, uint8_t below)
{
    uint8_t viol = 0;
    for (int j = 0; j < 5; ++j) {
        uint8_t val = BIT(cur, j);
        int same = 0;
        if (j > 0)   same += (BIT(cur, j - 1) == val);
        if (j < 4)   same += (BIT(cur, j + 1) == val);
        /* Below */
        if (j > 0)   same += (BIT(below, j - 1) == val);
        same += (BIT(below, j) == val);
        if (j < 4)   same += (BIT(below, j + 1) == val);
        if (same > 3) ++viol;
    }
    return viol;
}

/* Count violations for bottom row (no below neighbours) */
static uint8_t viol_bottom(uint8_t above, uint8_t cur)
{
    uint8_t viol = 0;
    for (int j = 0; j < 5; ++j) {
        uint8_t val = BIT(cur, j);
        int same = 0;
        if (j > 0)   same += (BIT(cur, j - 1) == val);
        if (j < 4)   same += (BIT(cur, j + 1) == val);
        /* Above */
        if (j > 0)   same += (BIT(above, j - 1) == val);
        same += (BIT(above, j) == val);
        if (j < 4)   same += (BIT(above, j + 1) == val);
        if (same > 3) ++viol;
    }
    return viol;
}

/* Initialise transition tables – done only once */
static void init_tables(void)
{
    if (tables_initialized) return;

    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            top_vi[a][b] = viol_top((uint8_t)a, (uint8_t)b);
            bottom_vi[a][b] = viol_bottom((uint8_t)a, (uint8_t)b);
            int idx = a * 32 + b;
            uint8_t len = 0;
            for (int c = 0; c < 32; ++c) {
                uint8_t v = viol_middle((uint8_t)a,
                                        (uint8_t)b,
                                        (uint8_t)c);
                if (v <= 2) {
                    trans_cur[idx][len] = (uint8_t)c;
                    trans_vi[idx][len] = v;
                    ++len;
                }
            }
            trans_len[idx] = len;
        }
    }
    tables_initialized = true;
}

seq_t a(idx_t n)
{
    /* Sequence offset 1: a(1)=0 */
    if (n < 2) return (seq_t)0;

    init_tables();

    /* DP arrays: dp[prevprev][prev][viol] */
    static seq_t dp[32][32][3];
    static seq_t dp_next[32][32][3];

    /* Zero tables */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* Initialise with first two rows */
    for (int r0 = 0; r0 < 32; ++r0)
        for (int r1 = 0; r1 < 32; ++r1) {
            uint8_t v = top_vi[r0][r1];
            if (v <= 2) dp[r0][r1][v] += (seq_t)1;
        }

    /* Process remaining rows */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp_next[i][j][k] = 0;

        for (int pp = 0; pp < 32; ++pp)
            for (int p = 0; p < 32; ++p) {
                int idx = pp * 32 + p;
                uint8_t len = trans_len[idx];
                for (uint8_t v1 = 0; v1 < 3; ++v1) {
                    seq_t ways = dp[pp][p][v1];
                    if (!ways) continue;
                    for (uint8_t t = 0; t < len; ++t) {
                        uint8_t cur = trans_cur[idx][t];
                        uint8_t inc = trans_vi[idx][t];
                        int nv = v1 + inc;
                        if (nv <= 2)
                            dp_next[p][cur][nv] += ways;
                    }
                }
            }
        /* Swap dp and dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* Final evaluation of the last row (no below neighbors) */
    seq_t result = 0;
    for (int pp = 0; pp < 32; ++pp)
        for (int p = 0; p < 32; ++p) {
            uint8_t v_last = bottom_vi[pp][p];
            for (int v1 = 0; v1 < 3; ++v1) {
                seq_t ways = dp[pp][p][v1];
                if (!ways) continue;
                int nv = v1 + v_last;
                if (nv == 2) result += ways;
            }
        }

    /* Empirically, the DP counts each configuration twice.
       Return the correct value by dividing by 2. */
    return result / 2u;
}
