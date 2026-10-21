/*
 * A278185  Number of nX4 0..3 arrays with every element plus 1 mod 4 equal to
 *          some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A278185 – Number of n×4 0..3 arrays with each element +1 (mod 4) equal to a neighbor at
   (-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,0); upper‑left element fixed to 0.
   Computation via transfer‑matrix DP over rows.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>   /* for malloc, free */

static uint8_t val[256][4];              /* decoded values of a row */
static uint8_t lr_sat[256];              /* bits satisfied in first row by left/right only */
static uint8_t prev_mask[256][256];      /* for pair (p,c) – bits where c[i]==(p[i]+1)%4 */
static uint8_t pass_mask[256][256];      /* for pair (p,c) – bits where c[i] satisfied by up/left/right */
static uint8_t new_mask[256][256];       /* (~pass_mask) & 0x0F */

/* transition data */
typedef struct { uint8_t nxt; uint8_t unmet; } Transition;
static Transition *trans_tbl = NULL;
static size_t trans_tot = 0;
static int trans_start[256][16];
static uint16_t trans_cnt[256][16];      /* use 16‑bit to hold up to 256 transitions */

/* DP buffers */
static seq_t dp_cur[256][16];
static seq_t dp_next[256][16];

static bool init_done = false;

static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* decode all rows */
    for (int id = 0; id < 256; ++id) {
        for (int j = 0; j < 4; ++j)
            val[id][j] = (uint8_t)((id >> (2*j)) & 3);
    }

    /* precompute left/right satisfaction mask for first row */
    for (int id = 0; id < 256; ++id) {
        uint8_t mask = 0;
        for (int i = 0; i < 4; ++i) {
            uint8_t need = (val[id][i] + 1) & 3;
            bool sat = false;
            if (i > 0 && val[id][i-1] == need) sat = true;
            if (i < 3 && val[id][i+1] == need) sat = true;
            if (sat) mask |= (1u << i);
        }
        lr_sat[id] = mask;
    }

    /* precompute masks for each pair of rows */
    for (int p = 0; p < 256; ++p) {
        for (int c = 0; c < 256; ++c) {
            uint8_t pm = 0;   /* bits where previous row is satisfied by current row */
            uint8_t pmask = 0;/* bits where current row is satisfied by left/right/up */
            for (int i = 0; i < 4; ++i) {
                uint8_t needCur = (val[c][i] + 1) & 3;
                uint8_t needPrev = (val[p][i] + 1) & 3;
                bool sat = false;
                if (i > 0 && val[c][i-1] == needCur) sat = true;
                if (i < 3 && val[c][i+1] == needCur) sat = true;
                if (val[p][i] == needCur) sat = true;
                if (i > 0 && val[p][i-1] == needCur) sat = true;
                if (i < 3 && val[p][i+1] == needCur) sat = true;
                if (sat) pmask |= (1u << i);
                if (val[c][i] == needPrev) pm |= (1u << i);
            }
            prev_mask[p][c] = pm;
            pass_mask[p][c] = pmask;
            new_mask[p][c]  = (~pmask) & 0x0F;
        }
    }

    /* count transitions for each (p,m) */
    for (int p = 0; p < 256; ++p) {
        for (int m = 0; m < 16; ++m) {
            unsigned int cnt = 0;
            for (int c = 0; c < 256; ++c) {
                if ((m & ~prev_mask[p][c]) == 0) ++cnt;
            }
            trans_cnt[p][m] = (uint16_t)cnt;
            trans_start[p][m] = (int)trans_tot;
            trans_tot += cnt;
        }
    }

    /* allocate transition table */
    trans_tbl = (Transition*)malloc(trans_tot * sizeof(Transition));
    if (!trans_tbl) return;   /* abort on allocation failure */

    /* fill transition table */
    for (int p = 0; p < 256; ++p) {
        for (int m = 0; m < 16; ++m) {
            int idx = trans_start[p][m];
            for (int c = 0; c < 256; ++c) {
                if ((m & ~prev_mask[p][c]) == 0) {
                    trans_tbl[idx].nxt   = (uint8_t)c;
                    trans_tbl[idx].unmet = new_mask[p][c];
                    ++idx;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;

    if (n == 1) {
        /* Count rows with upper-left 0 and all cells satisfied by left/right */
        seq_t res = 0;
        for (int r = 0; r < 256; ++r) {
            if (val[r][0] != 0) continue;
            if ((~lr_sat[r]) & 0x0F) continue;
            res += 1;
        }
        return res;
    }

    memset(dp_cur, 0, sizeof(dp_cur));

    /* initialize with first row */
    for (int r = 0; r < 256; ++r) {
        if (val[r][0] != 0) continue;
        uint8_t m0 = (~lr_sat[r]) & 0x0F;
        dp_cur[r][m0] += 1;
    }

    /* process remaining rows (n-1 iterations) */
    for (idx_t row = 1; row < n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int p = 0; p < 256; ++p) {
            for (int m = 0; m < 16; ++m) {
                seq_t cur = dp_cur[p][m];
                if (cur == 0) continue;
                int start = trans_start[p][m];
                int cnt   = trans_cnt[p][m];
                for (int t = 0; t < cnt; ++t) {
                    const Transition *tr = &trans_tbl[start + t];
                    dp_next[tr->nxt][tr->unmet] += cur;
                }
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* sum over states where last row has no unmet columns */
    seq_t result = 0;
    for (int r = 0; r < 256; ++r)
        result += dp_cur[r][0];
    return result;
}
