/*
 * A253835  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          sum plus antidiagonal sum nondecreasing horizontally, vertically
 *          and ne-to-sw antidiagonally.
 * https://oeis.org/A253835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dynamic programming over rows, counting binary arrays of size
 *            (n+2)×4 with nondecreasing diagonal+antidiagonal sums
 *            horizontally, vertically, and along the NE→SW anti‑diagonal
 * Range:     u8 none, u16 n <= 4, u32 n <= 29, u64 n <= 93, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM        6           /* maximum possible sum of one diagonal+anti-diagonal */
#define SUM_RANGE      7           /* 0..6 inclusive */
#define PAIR_COUNT     28          /* number of (s0,s1) with 0<=s0<=s1<=6 */
#define ROWS_4         16          /* 4-bit rows (0..15) */
#define STATE_COUNT    (ROWS_4*ROWS_4*PAIR_COUNT)  /* 16*16*28 = 7168 states */
#define MAX_TRANS      16          /* at most 16 possible new rows per state */

/* pre‑computed mapping from (s0,s1) to a compact index (0..27) */
static int pair_from_sum[SUM_RANGE][SUM_RANGE];
static uint8_t pair_s0[PAIR_COUNT];
static uint8_t pair_s1[PAIR_COUNT];

/* sums for each triple of rows: s0 (col j=0), s1 (col j=1) */
static uint8_t sum0[ROWS_4][ROWS_4][ROWS_4];
static uint8_t sum1[ROWS_4][ROWS_4][ROWS_4];

/* transitions: next states reachable from each state */
static int trans_next[STATE_COUNT][MAX_TRANS];
static uint8_t trans_count[STATE_COUNT];

/* once‑only initialisation */
static bool preinit_done = false;
static void preinit(void)
{
    if (preinit_done) return;

    /* build pair mapping */
    int idx = 0;
    for (uint8_t s0 = 0; s0 < SUM_RANGE; ++s0)
        for (uint8_t s1 = s0; s1 < SUM_RANGE; ++s1)
        {
            pair_from_sum[s0][s1] = idx;
            pair_s0[idx] = s0;
            pair_s1[idx] = s1;
            ++idx;
        }
    for (uint8_t s0 = 0; s0 < SUM_RANGE; ++s0)
        for (uint8_t s1 = 0; s1 < SUM_RANGE; ++s1)
            if (s1 < s0) pair_from_sum[s0][s1] = -1;  /* invalid pair */

    /* precompute sums for every triple of rows */
    for (int r0 = 0; r0 < ROWS_4; ++r0)
        for (int r1 = 0; r1 < ROWS_4; ++r1)
            for (int r2 = 0; r2 < ROWS_4; ++r2)
            {
                /* bits of each row */
                uint8_t b0[4], b1[4], b2[4];
                for (int c = 0; c < 4; ++c)
                {
                    b0[c] = (r0 >> (3 - c)) & 1;
                    b1[c] = (r1 >> (3 - c)) & 1;
                    b2[c] = (r2 >> (3 - c)) & 1;
                }
                /* S0 uses columns 0 and 2 */
                sum0[r0][r1][r2] =
                    b0[0] + b0[2] + b2[0] + b2[2] + 2 * b1[1];
                /* S1 uses columns 1 and 3 */
                sum1[r0][r1][r2] =
                    b0[1] + b0[3] + b2[1] + b2[3] + 2 * b1[2];
            }

    /* precompute transitions for every state */
    for (int pr = 0; pr < ROWS_4; ++pr)
        for (int cr = 0; cr < ROWS_4; ++cr)
            for (int bi = 0; bi < PAIR_COUNT; ++bi)
            {
                int state_idx = ((pr * ROWS_4) + cr) * PAIR_COUNT + bi;
                uint8_t sp0 = pair_s0[bi];
                uint8_t sp1 = pair_s1[bi];
                uint8_t cnt = 0;
                for (int nr = 0; nr < ROWS_4; ++nr)
                {
                    uint8_t cur0 = sum0[pr][cr][nr];
                    uint8_t cur1 = sum1[pr][cr][nr];

                    /* horizontal */
                    if (cur0 > cur1) continue;
                    /* vertical */
                    if (sp0 > cur0) continue;
                    if (sp1 > cur1) continue;
                    /* NE→SW */
                    if (sp1 > cur0) continue;

                    int nxt_bi = pair_from_sum[cur0][cur1];
                    if (nxt_bi == -1) continue;  /* should not happen */
                    int nxt_state = ((cr * ROWS_4) + nr) * PAIR_COUNT + nxt_bi;
                    trans_next[state_idx][cnt++] = nxt_state;
                }
                trans_count[state_idx] = cnt;
            }

    preinit_done = true;
}

/* DP arrays (two buffers for rolling computation) */
static seq_t dp_a[STATE_COUNT];
static seq_t dp_b[STATE_COUNT];

seq_t a(idx_t n)  /* external linkage */
{
    preinit();

    /* number of rows is n + 2 */
    idx_t rows = n + 2;
    if (rows < 3) return 0;  /* not used for n>=1 */

    /* choose buffers */
    seq_t *dp_cur = dp_a;
    seq_t *dp_next = dp_b;

    /* clear dp_cur */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* initialise with all triples r0,r1,r2 (first 3 rows) */
    for (int r0 = 0; r0 < ROWS_4; ++r0)
        for (int r1 = 0; r1 < ROWS_4; ++r1)
            for (int r2 = 0; r2 < ROWS_4; ++r2)
            {
                uint8_t s0 = sum0[r0][r1][r2];
                uint8_t s1 = sum1[r0][r1][r2];
                if (s0 > s1) continue;            /* horizontal nondecreasing */
                int pair_idx = pair_from_sum[s0][s1];
                int state_idx = ((r1 * ROWS_4) + r2) * PAIR_COUNT + pair_idx;
                dp_cur[state_idx] += 1;             /* count one array for this triple */
            }

    /* DP over the remaining rows (from index 3 to rows-1) */
    for (idx_t r = 3; r < rows; ++r)
    {
        /* zero next buffer */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s)
        {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            uint8_t tr = trans_count[s];
            for (int t = 0; t < tr; ++t)
            {
                int nxt = trans_next[s][t];
                dp_next[nxt] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* sum all final state counts */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp_cur[i];
    return ans;
}
