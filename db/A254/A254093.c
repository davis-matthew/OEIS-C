/*
 * A254093  Number of (2+2)X(n+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal maximum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming (no simple closed‑form)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   compute_d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_d.h"

/*------------------------------------------------------------*/
/*  Transition data for 3×3 blocks                           */
/*------------------------------------------------------------*/
#define MAX_COLS 16               /* 4 rows → 4 bits per column (0..15) */
#define SENTINEL 3                /* sentinel for undefined D values (stored as 3) */

/* Transition entry: new column mask and D values for rows 0 and 1 */
typedef struct {
    uint8_t  col;                  /* new column mask */
    int8_t   d0;                   /* diagonal max – antidiagonal max for row 0 */
    int8_t   d1;                   /* same for row 1 */
} trans_t;

static trans_t  trans[MAX_COLS][MAX_COLS][MAX_COLS];  /* precomputed transitions */
static uint8_t  trans_cnt[MAX_COLS][MAX_COLS];       /* number of valid transitions per (a,b) */
static int      precomp_done = 0;                    /* flag for precomputation */

/* Pre‑compute all admissible transitions */
static void init_precomp(void)
{
    int a, b, c, d0, d1, t;
    for (a = 0; a < MAX_COLS; ++a) {
        for (b = 0; b < MAX_COLS; ++b) {
            t = 0;
            for (c = 0; c < MAX_COLS; ++c) {
                d0 = compute_d(a, b, c, 0);
                d1 = compute_d(a, b, c, 1);
                if (d0 <= d1) {                       /* vertical nondecreasing */
                    trans[a][b][t].col = (uint8_t)c;
                    trans[a][b][t].d0  = (int8_t)d0;
                    trans[a][b][t].d1  = (int8_t)d1;
                    ++t;
                }
            }
            trans_cnt[a][b] = t;
        }
    }
    precomp_done = 1;
}

/* Dynamic programming to count arrays for a given n */
static seq_t dp_count(idx_t n)
{
    if (!precomp_done) init_precomp();

    /* DP tables: dp[a][b][da][db]  (da,db: 0..3, 3 = sentinel) */
    static seq_t dp[MAX_COLS][MAX_COLS][4][4];
    static seq_t ndp[MAX_COLS][MAX_COLS][4][4];
    int a, b, da, db, t;
    seq_t cnt;

    /* clear dp */
    for (a = 0; a < MAX_COLS; ++a)
        for (b = 0; b < MAX_COLS; ++b)
            for (da = 0; da < 4; ++da)
                for (db = 0; db < 4; ++db)
                    dp[a][b][da][db] = 0;

    /* initial state: two arbitrary initial columns, D undefined */
    for (a = 0; a < MAX_COLS; ++a)
        for (b = 0; b < MAX_COLS; ++b)
            dp[a][b][SENTINEL][SENTINEL] = 1;

    unsigned int steps = (unsigned int)n;   /* number of subblocks (n) */

    for (t = 0; t < steps; ++t) {
        /* clear ndp */
        for (a = 0; a < MAX_COLS; ++a)
            for (b = 0; b < MAX_COLS; ++b)
                for (da = 0; da < 4; ++da)
                    for (db = 0; db < 4; ++db)
                        ndp[a][b][da][db] = 0;

        for (a = 0; a < MAX_COLS; ++a) {
            for (b = 0; b < MAX_COLS; ++b) {
                for (da = 0; da < 4; ++da) {
                    for (db = 0; db < 4; ++db) {
                        cnt = dp[a][b][da][db];
                        if (!cnt) continue;

                        int prev0 = (da == SENTINEL) ? 0 : da - 1;
                        int prev1 = (db == SENTINEL) ? 0 : db - 1;

                        for (int k = 0; k < trans_cnt[a][b]; ++k) {
                            trans_t *tr = &trans[a][b][k];
                            int d0 = tr->d0;
                            int d1 = tr->d1;

                            /* horizontal nondecreasing */
                            if (da != SENTINEL && d0 < prev0) continue;
                            if (db != SENTINEL && d1 < prev1) continue;
                            /* NE‑SW antidiagonal constraint */
                            if (db != SENTINEL && d0 > prev1) continue;

                            int nd0 = d0 + 1;   /* map -1,0,1 to 0,1,2 */
                            int nd1 = d1 + 1;
                            ndp[b][tr->col][nd0][nd1] += cnt;
                        }
                    }
                }
            }
        }

        /* swap dp and ndp */
        for (a = 0; a < MAX_COLS; ++a)
            for (b = 0; b < MAX_COLS; ++b)
                for (da = 0; da < 4; ++da)
                    for (db = 0; db < 4; ++db) {
                        dp[a][b][da][db] = ndp[a][b][da][db];
                        ndp[a][b][da][db] = 0;
                    }
    }

    /* sum over terminal states with defined D values */
    seq_t result = 0;
    for (a = 0; a < MAX_COLS; ++a)
        for (b = 0; b < MAX_COLS; ++b)
            for (da = 0; da < 3; ++da)
                for (db = 0; db < 3; ++db)
                    result += dp[a][b][da][db];

    return result;
}

/*------------------------------------------------------------*/
/*  Public function implementing sequence A254093             */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n is 1‑based index; DP counts configurations for width n */
    return dp_count(n);
}
