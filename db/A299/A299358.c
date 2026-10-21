/*
 * A299358  Number of nX7 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with upper‑left element 0
 *            whose king‑move adjacent same‑value neighbours equals 0,1,2,3,6
 *            or 8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*--- constants ---------------------------------------------------*/
#define WIDTH      7
#define MASKS      128                  /* 2^7 */
#define STATES     (MASKS * MASKS)      /* pair of rows */

/*--- globals for tables ------------------------------------------*/
static bool allowed[9];
static bool middle_valid[MASKS][MASKS][MASKS];
static bool top_valid[MASKS][MASKS];
static bool bottom_valid[MASKS][MASKS];

static uint32_t transition_start[STATES];
static uint32_t *transition_next = NULL;   /* flattened list of next rows */
static uint32_t  *transition_count = NULL; /* number of transitions per state */

static bool built = false;

/*--- elementary helpers ------------------------------------------*/
static bool row_valid_int(int prev, bool has_prev,
                          int  cur, bool has_next,
                          int  next)
{
    static const int drow[8] = {-1,-1,-1,0,0,1,1,1};
    static const int dcol[8] = {-1,0,1,-1,1,-1,0,1};

    for (int col = 0; col < WIDTH; ++col) {
        int val = (cur >> col) & 1;
        int cnt = 0;
        for (int k = 0; k < 8; ++k) {
            int nc = col + dcol[k];
            if (nc < 0 || nc >= WIDTH) continue;
            int nval;
            if (drow[k] == -1) {
                if (!has_prev) continue;
                nval = (prev >> nc) & 1;
            } else if (drow[k] == 1) {
                if (!has_next) continue;
                nval = (next >> nc) & 1;
            } else {
                nval = (cur >> nc) & 1;
            }
            if (nval == val) ++cnt;
        }
        if (!allowed[cnt]) return false;
    }
    return true;
}

/*--- table construction ------------------------------------------*/
static void init_allowed(void)
{
    for (int i = 0; i < 9; ++i) allowed[i] = false;
    allowed[0] = allowed[1] = allowed[2]   = true;
    allowed[3] = allowed[6] = allowed[8]   = true;
}

static void build_tables(void)
{
    init_allowed();

    /* middle_valid */
    for (int a = 0; a < MASKS; ++a)
        for (int b = 0; b < MASKS; ++b)
            for (int c = 0; c < MASKS; ++c)
                middle_valid[a][b][c] =
                    row_valid_int(a, true,  c, true, b);

    /* top and bottom validity */
    for (int a = 0; a < MASKS; ++a)
        for (int b = 0; b < MASKS; ++b) {
            top_valid[a][b]    = row_valid_int(0, false, a, true, b);
            bottom_valid[a][b] = row_valid_int(a, true,  b, false, 0);
        }

    /* transition counts */
    transition_count = (uint32_t*)malloc(STATES * sizeof(uint32_t));
    if (!transition_count) abort();
    uint32_t total_trans = 0;
    for (int i = 0; i < STATES; ++i) {
        int prev = i >> WIDTH;
        int cur  = i & (MASKS - 1);
        uint32_t cnt = 0;
        for (int n = 0; n < MASKS; ++n)
            if (middle_valid[prev][n][cur]) ++cnt;
        transition_count[i] = cnt;
        transition_start[i] = total_trans;
        total_trans += cnt;
    }

    transition_next = (uint32_t*)malloc(total_trans * sizeof(uint32_t));
    if (!transition_next) abort();

    /* fill transition list */
    for (int i = 0; i < STATES; ++i) {
        int prev = i >> WIDTH;
        int cur  = i & (MASKS - 1);
        uint32_t pos = transition_start[i];
        for (int n = 0; n < MASKS; ++n)
            if (middle_valid[prev][n][cur])
                transition_next[pos++] = n;
    }
}

/*--- state tables for DP ------------------------------------------*/
static seq_t *dp = NULL;     /* current states */
static seq_t *ndp = NULL;    /* next states */
static int   active[STATES];
static int   new_active[STATES];
static bool  visited[STATES];

/*--- main function -----------------------------------------------*/
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)64;   /* only upper-left cell fixed to 0 */

    /* initialise tables once */
    if (!built) {
        build_tables();
        dp  = (seq_t*)malloc(STATES * sizeof(seq_t));
        ndp = (seq_t*)malloc(STATES * sizeof(seq_t));
        if (!dp || !ndp) abort();
        /* zero init */
        for (int i = 0; i < STATES; ++i) { dp[i] = 0; ndp[i] = 0; }
        built = true;
    }

    /* setup first two rows */
    int active_len = 0;
    for (int r0 = 0; r0 < MASKS; ++r0) {
        if (r0 & 1) continue;              /* upper‑left must be 0 */
        for (int r1 = 0; r1 < MASKS; ++r1)
            if (top_valid[r0][r1]) {
                int idx = (r0 << WIDTH) | r1;
                dp[idx] = 1;
                active[active_len++] = idx;
            }
    }

    if (n == 2) {                /* finish immediately */
        seq_t ans = 0;
        for (int i = 0; i < active_len; ++i) {
            int idx = active[i];
            int a = idx >> WIDTH;
            int b = idx & (MASKS - 1);
            if (bottom_valid[a][b]) ans += dp[idx];
        }
        return ans;
    }

    /* DP over remaining rows (row index 2 .. n-1) */
    for (idx_t row = 2; row < n; ++row) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) ndp[i] = 0;
        memset(visited, 0, sizeof(visited));
        int new_len = 0;

        for (int i = 0; i < active_len; ++i) {
            int idx = active[i];
            seq_t cur_val = dp[idx];
            int prev = idx >> WIDTH;
            int cur  = idx & (MASKS - 1);
            uint32_t s  = transition_start[idx];
            uint32_t cnt = transition_count[idx];
            for (uint32_t t = 0; t < cnt; ++t) {
                int nxt = transition_next[s + t];
                int nidx = (cur << WIDTH) | nxt;
                if (!visited[nidx]) { visited[nidx] = true; new_active[new_len++] = nidx; }
                ndp[nidx] += cur_val;
            }
        }

        /* swap dp and ndp */
        seq_t *tmp = dp; dp = ndp; ndp = tmp;

        /* prepare active list for next iteration */
        memcpy(active, new_active, new_len * sizeof(int));
        active_len = new_len;
    }

    /* final counting: only bottom row must satisfy constraint */
    seq_t ans = 0;
    for (int i = 0; i < active_len; ++i) {
        int idx = active[i];
        int a = idx >> WIDTH;
        int b = idx & (MASKS - 1);
        if (bottom_valid[a][b]) ans += dp[idx];
    }
    return ans;
}
