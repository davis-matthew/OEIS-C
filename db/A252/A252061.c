/*
 * A252061  Number of (n+2)X(4+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 2 or 4.
 * https://oeis.org/A252061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+2)×6 arrays of entries 0,1,2 in which every 3×3
 *            subblock has no row, column, main‑diagonal or anti‑diagonal sum
 *            equal to 1, 2 or 4. The function a(n) returns this count for n ≥
 *            1
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 729                 /* 3^6 possible rows */
#define STATE_COUNT (ROWS*ROWS)  /* (r0,r1) pairs */

/* Pre‑computed data structures */
typedef struct {
    uint16_t *next;   /* list of neighbouring r2 values */
    uint16_t  cnt;    /* number of neighbours, 0xFFFF = not computed */
} PairAdj;

static uint8_t  g_digits[ROWS][6];            /* digit representation of row */
static uint8_t  g_rowSeg[ROWS][4];            /* sums of 3 consecutive columns */
static PairAdj *g_adj;                         /* adjacency per pair */

/* DP buffers and active state lists (reused across calls) */
static seq_t *g_dpA = NULL;   /* holds counts for current states */
static seq_t *g_dpB = NULL;   /* buffer for next states           */
static uint32_t *g_actA = NULL;
static uint32_t *g_actB = NULL;

/* --------------------------------------------------------------------- */
/* Initialise static tables (executed once) */
static void init_tables(void)
{
    g_adj      = (PairAdj*)malloc(sizeof(PairAdj)*STATE_COUNT);
    g_dpA      = (seq_t*) malloc(sizeof(seq_t)*STATE_COUNT);
    g_dpB      = (seq_t*) malloc(sizeof(seq_t)*STATE_COUNT);
    g_actA     = (uint32_t*)malloc(sizeof(uint32_t)*STATE_COUNT);
    g_actB     = (uint32_t*)malloc(sizeof(uint32_t)*STATE_COUNT);

    /* compute digits and row segment sums */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int c = 0; c < 6; ++c) {
            g_digits[r][c] = (uint8_t)(x % 3);
            x /= 3;
        }
        for (int j = 0; j < 4; ++j) {
            g_rowSeg[r][j] =
                g_digits[r][j] + g_digits[r][j+1] + g_digits[r][j+2];
        }
    }

    /* initialise adjacency list sentinels */
    for (int i = 0; i < STATE_COUNT; ++i) {
        g_adj[i].next = NULL;
        g_adj[i].cnt  = 0xFFFF; /* not yet computed */
    }

    /* zero DP buffers */
    memset(g_dpA, 0, sizeof(seq_t)*STATE_COUNT);
    memset(g_dpB, 0, sizeof(seq_t)*STATE_COUNT);
}

/* --------------------------------------------------------------------- */
/* Test whether row r[0]..r[2] together with r[0],r[1],r[2] satisfy all
   constraints on every 3×3 subblock. */
static int valid_triplet(int r0, int r1, int r2)
{
    /* forbidden sums: 1, 2, 4 */
    static const uint8_t forbid[7] = {0,1,1,0,1,0,0};

    for (int j = 0; j < 4; ++j) {
        /* row sums of the 3 rows */
        uint8_t s0 = g_rowSeg[r0][j];
        uint8_t s1 = g_rowSeg[r1][j];
        uint8_t s2 = g_rowSeg[r2][j];
        if (forbid[s0] || forbid[s1] || forbid[s2]) return 0;

        /* column sums of the 3 columns */
        uint8_t c0 = g_digits[r0][j]   + g_digits[r1][j]   + g_digits[r2][j];
        if (forbid[c0]) return 0;
        uint8_t c1 = g_digits[r0][j+1] + g_digits[r1][j+1] + g_digits[r2][j+1];
        if (forbid[c1]) return 0;
        uint8_t c2 = g_digits[r0][j+2] + g_digits[r1][j+2] + g_digits[r2][j+2];
        if (forbid[c2]) return 0;

        /* main and anti diagonals */
        uint8_t d  = g_digits[r0][j]     + g_digits[r1][j+1] + g_digits[r2][j+2];
        if (forbid[d]) return 0;
        uint8_t ad = g_digits[r0][j+2]   + g_digits[r1][j+1] + g_digits[r2][j];
        if (forbid[ad]) return 0;
    }
    return 1;
}

/* --------------------------------------------------------------------- */
/* Lazily compute adjacency list for pair (r0,r1). */
static void compute_adj(int pair_idx, int r0, int r1)
{
    if (g_adj[pair_idx].cnt != 0xFFFF) return;   /* already computed */

    uint16_t temp[ROWS];
    int cnt = 0;
    for (int r2 = 0; r2 < ROWS; ++r2) {
        if (valid_triplet(r0, r1, r2))
            temp[cnt++] = (uint16_t)r2;
    }
    if (cnt == 0) {
        g_adj[pair_idx].next = NULL;
        g_adj[pair_idx].cnt  = 0;
        return;
    }

    uint16_t *arr = (uint16_t*)malloc(cnt * sizeof(uint16_t));
    memcpy(arr, temp, cnt*sizeof(uint16_t));
    g_adj[pair_idx].next = arr;
    g_adj[pair_idx].cnt  = (uint16_t)cnt;
}

/* --------------------------------------------------------------------- */
/* Main function required by the harness */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }
    if (n < 1) return 0;                 /* undefined below offset */

    /* ensure dpB buffer is zeroed for each new call (otherwise stale data from
       previous calls would corrupt the result) */
    for (size_t i = 0; i < (size_t)STATE_COUNT; ++i) g_dpB[i] = 0;

    /* number of rows in the array */
    idx_t rows = n + 2;

    /* initialise dpA with all possible first two rows */
    uint32_t act_cnt = 0;
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int idx = r0 * ROWS + r1;
            g_dpA[idx] = (seq_t)1;
            g_actA[act_cnt++] = (uint32_t)idx;
        }

    /* DP buffers and active lists */
    seq_t *dp_cur = g_dpA;
    seq_t *dp_nxt = g_dpB;
    uint32_t *act_cur  = g_actA;
    uint32_t *act_nxt  = g_actB;

    /* iterate over the remaining rows */
    for (int step = 2; step < rows; ++step) {
        uint32_t act_next_cnt = 0;

        /* process all current states */
        for (uint32_t ai = 0; ai < act_cnt; ++ai) {
            int idx = act_cur[ai];
            seq_t cnt = dp_cur[idx];
            if (cnt == 0) continue;

            int r0 = idx / ROWS;
            int r1 = idx % ROWS;

            /* compute adjacency once per pair */
            compute_adj(idx, r0, r1);
            PairAdj *pa = &g_adj[idx];
            uint16_t *nbr = pa->next;
            uint16_t nb_cnt = pa->cnt;

            /* transition to next state (r1,r2) */
            for (uint16_t k = 0; k < nb_cnt; ++k) {
                int new_idx = r1 * ROWS + nbr[k];
                if (dp_nxt[new_idx] == 0)
                    act_nxt[act_next_cnt++] = (uint32_t)new_idx;
                dp_nxt[new_idx] += cnt;
            }
        }

        /* clear dp_cur entries (they are no longer needed) */
        for (uint32_t ai = 0; ai < act_cnt; ++ai)
            dp_cur[ act_cur[ai] ] = 0;

        /* swap buffers and active lists */
        seq_t *tmp_dp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp_dp;
        uint32_t *tmp_act = act_cur; act_cur = act_nxt; act_nxt = tmp_act;
        act_cnt = act_next_cnt;
    }

    /* sum remaining counts to produce the final result */
    seq_t result = 0;
    for (uint32_t ai = 0; ai < act_cnt; ++ai)
        result += dp_cur[ act_cur[ai] ];

    return result;
}
