/*
 * A295118  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 4 1s.
 * https://oeis.org/A295118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A295118
 *  Number of n×6 binary arrays in which every 1 has
 *  exactly one or exactly four orthogonal neighbours that are 1.
 *
 *  The algorithm uses transfer‑matrix dynamic programming over rows.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* forward declarations supplied by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Pre‑computed data structures                                    */
/* ------------------------------------------------------------------ */

/* valid[up][curr][down] is true iff for every 1 in 'curr'
 * the number of orthogonal neighbours that are 1 is 1 or 4. */
static bool valid[64][64][64];

/* Transition lists:
 * For each state s = (prev,cur) encoded as prev*64+cur,
 * next_offset[s] gives the start index in next_flat,
 * next_count[s] the number of reachable next states,
 * next_flat stores the flattened list of reachable next states
 * (each encoded as cur*64+next).  The maximum possible size is
 * 64*64*64 = 262144 entries. */
static int next_offset[4096];
static int next_count[4096];
static int next_flat[262144];

/* Initialization flag */
static int initialized = 0;

/* ------------------------------------------------------------------ */
/*  Initialization routine                                            */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    if (initialized) return;

    /* Pre‑compute validity table */
    for (int up = 0; up < 64; ++up) {
        for (int cur = 0; cur < 64; ++cur) {
            for (int down = 0; down < 64; ++down) {
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j) {
                    if ((cur >> j) & 1) {
                        int deg = 0;
                        /* horizontal neighbours */
                        if (j > 0 && ((cur >> (j - 1)) & 1)) ++deg;
                        if (j + 1 < 6 && ((cur >> (j + 1)) & 1)) ++deg;
                        /* vertical neighbours */
                        if ((up >> j) & 1) ++deg;
                        if ((down >> j) & 1) ++deg;
                        if (deg != 1 && deg != 4) ok = false;
                    }
                }
                valid[up][cur][down] = ok;
            }
        }
    }

    /* Build transition lists */
    int off = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            int state = (prev << 6) | cur;
            next_offset[state] = off;
            int cnt = 0;
            for (int next = 0; next < 64; ++next) {
                if (valid[prev][cur][next]) {
                    next_flat[off++] = (cur << 6) | next;
                    ++cnt;
                }
            }
            next_count[state] = cnt;
        }
    }

    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Main DP routine                                                  */
/* ------------------------------------------------------------------ */
static seq_t dp[4096];
static seq_t next_dp[4096];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n = 1 */

    init_tables();

    /* initialise dp with states (0, r) = 1 for all r */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;
    for (int r = 0; r < 64; ++r) dp[r] = 1;   /* state = 0*64 + r */

    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < 64; ++r) {
            if (valid[0][r][0]) res += dp[r];
        }
        return res;
    }

    /* DP for rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 4096; ++i) next_dp[i] = 0;
        for (int state = 0; state < 4096; ++state) {
            seq_t v = dp[state];
            if (v == 0) continue;
            int off = next_offset[state];
            int cnt = next_count[state];
            const int *list = next_flat + off;
            for (int t = 0; t < cnt; ++t) {
                int next_state = list[t];
                next_dp[next_state] += v;
            }
        }
        /* copy next_dp back into dp */
        for (int i = 0; i < 4096; ++i) dp[i] = next_dp[i];
    }

    /* final validation with dummy bottom row 0 */
    seq_t result = 0;
    for (int state = 0; state < 4096; ++state) {
        seq_t v = dp[state];
        if (v == 0) continue;
        int prev = state >> 6;
        int cur = state & 63;
        if (valid[prev][cur][0]) result += v;
    }
    return result;
}
