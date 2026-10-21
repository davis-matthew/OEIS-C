/*
 * A295049  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1s.
 * https://oeis.org/A295049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A295049 - Number of n×6 0..1 arrays with each 1
    horizontally or vertically adjacent to 0 or 2 1s.
    The implementation uses dynamic programming over two
    consecutive rows.  For each pair (prev,cur) of rows
    we pre‑compute all admissible next rows, storing
    the transitions in a compact table.  The function
    works for any seq_t/idx_t types provided by the
    build.                                                     */

#include <stdbool.h>
#include <stdint.h>

/* -------------  Transition tables  ----------------- */
static bool tables_ready = false;

static int8_t horiz_adj[64][6];                 /* horizontal neighbours */
static uint16_t trans_next[4096 * 64];          /* next states indices */
static uint8_t  trans_len[4096];                /* number of transitions per state */
static uint32_t trans_start[4096];              /* offset of transitions for each state */

static void build_tables(void)
{
    if (tables_ready) return;
    /* horizontal neighbours for each row mask */
    for (int mask = 0; mask < 64; ++mask) {
        for (int j = 0; j < 6; ++j) {
            int cnt = 0;
            if (j > 0 && (mask & (1 << (j - 1)))) ++cnt;
            if (j < 5 && (mask & (1 << (j + 1)))) ++cnt;
            horiz_adj[mask][j] = (int8_t)cnt;
        }
    }
    /* transitions for all pairs (pp, p) */
    uint32_t pos = 0;          /* current position in trans_next */
    for (int pp = 0; pp < 64; ++pp) {
        for (int p  = 0; p < 64; ++p) {
            int state = pp * 64 + p;
            trans_start[state] = pos;
            bool ok = true;
            int forcedMask = 0;         /* bits forced to 1 in the next row */
            int freeMask   = 0;         /* bits where p has 0 – free in next row */
            for (int j = 0; j < 6; ++j) {
                if (p & (1 << j)) {
                    int base = horiz_adj[p][j] + ((pp >> j) & 1);
                    if (base == 3) { ok = false; break; }
                    if (base == 1) forcedMask |= (1 << j);
                    /* base==0 or 2 → down neighbor forced to 0 */
                } else {
                    freeMask |= (1 << j);
                }
            }
            if (!ok) { trans_len[state] = 0; continue; }

            /* enumerate all subsets of freeMask */
            int sub = freeMask;
            do {
                int nxt = forcedMask | sub;
                trans_next[pos++] = (uint16_t)((p << 6) | nxt);
                ++trans_len[state];
                if (sub == 0) break;
                sub = (sub - 1) & freeMask;
            } while (1);
        }
    }
    tables_ready = true;
}

/* -----------------  Main function  ----------------- */
seq_t a(idx_t n)
{
    build_tables();

    /* DP buffers */
    static seq_t dp_buf[4096];
    static seq_t ndp_buf[4096];
    seq_t *dp  = dp_buf;
    seq_t *ndp = ndp_buf;

    /* initialise all to zero */
    for (int i = 0; i < 4096; ++i) { dp[i] = 0; ndp[i] = 0; }

    /* first row: previous row is 0 */
    for (int cur = 0; cur < 64; ++cur) dp[cur] = 1;          /* state 0*64+cur */

    /* add remaining rows 1 .. n-1 */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < 4096; ++i) ndp[i] = 0;
        for (int state = 0; state < 4096; ++state) {
            seq_t ways = dp[state];
            if (!ways) continue;
            uint32_t start = trans_start[state];
            uint8_t  len   = trans_len[state];
            for (uint8_t i = 0; i < len; ++i) {
                ndp[ trans_next[start + i] ] += ways;
            }
        }
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    /* final check – last row has no downward neighbour */
    seq_t result = 0;
    for (int pp = 0; pp < 64; ++pp) {
        for (int p  = 0; p < 64; ++p) {
            seq_t ways = dp[pp * 64 + p];
            if (!ways) continue;
            bool ok = true;
            for (int j = 0; j < 6 && ok; ++j) {
                if (p & (1 << j)) {
                    int base = horiz_adj[p][j] + ((pp >> j) & 1);
                    if (base != 0 && base != 2) ok = false;
                }
            }
            if (ok) result += ways;
        }
    }
    return result;
}
