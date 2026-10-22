/*
 * A304345  Number of nX6 0..1 arrays with every element unequal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A304345 : Number of n×6 binary arrays with the
   constraint that for every cell the number of king‑move adjacent
   cells with a different value is one of {0,1,2,4,5}.  The
   upper‑left cell of the first row is fixed to 0.  This
   implementation uses a transfer‑matrix based DP over consecutive
   rows; the state is a pair of consecutive rows (first 6‑bit
   patterns).  The algorithm runs in O(n·64³) time with a small
   constant (≈50 M operations for n≈10 000).  It works for any
   integer type seq_t/idx_t provided by the build system. */

/*< included headers >*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper constants */
#define COLS 6
#define ROW_PAT 64      /* 6 bit patterns: 0 … 63 */
#define STATE_CNT 4096  /* 64×64 states */

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/*====================================================================*/
/*  Precomputation of transition lists                               */
/*====================================================================*/
static bool top_next_exists[ROW_PAT][ROW_PAT];
static uint8_t top_next_list[ROW_PAT][ROW_PAT];
static uint8_t top_next_count[ROW_PAT];

static uint8_t middle_next_list[STATE_CNT][ROW_PAT];
static uint8_t middle_next_count[STATE_CNT];

static bool last_valid[STATE_CNT];

/*--------------------------------------------------------------------*/
/*  Row‑validation routine                                           */
/*--------------------------------------------------------------------*/
static inline bool row_valid(uint8_t above, uint8_t middle, uint8_t below,
                             bool has_above, bool has_below)
{
    for (int c = 0; c < COLS; ++c) {
        uint8_t val = (middle >> c) & 1u;
        int unequal = 0;

        for (int dr = -1; dr <= 1; ++dr) {
            uint8_t neigh_pat;
            if (dr == -1) {
                if (!has_above) continue;
                neigh_pat = above;
            } else if (dr == 0) {
                neigh_pat = middle;
            } else { /* dr == 1 */
                if (!has_below) continue;
                neigh_pat = below;
            }

            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = c + dc;
                if (nc < 0 || nc >= COLS) continue;
                uint8_t neigh_val = (neigh_pat >> nc) & 1u;
                if (neigh_val != val) ++unequal;
            }
        }

        if (!(unequal == 0 || unequal == 1 || unequal == 2 ||
              unequal == 4 || unequal == 5))
            return false;
    }
    return true;
}

/*--------------------------------------------------------------------*/
/*  One‑time initialization                                          */
/*--------------------------------------------------------------------*/
static void init_precomp(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Top‑row transitions (no above row, below exists) */
    for (int p0 = 0; p0 < ROW_PAT; ++p0) {
        int cnt = 0;
        for (int p1 = 0; p1 < ROW_PAT; ++p1) {
            if (row_valid(0, (uint8_t)p0, (uint8_t)p1, false, true)) {
                top_next_list[p0][cnt++] = (uint8_t)p1;
            }
        }
        top_next_count[p0] = (uint8_t)cnt;
    }

    /* Middle‑row transitions (both above and below exist) */
    for (int a = 0; a < ROW_PAT; ++a) {
        for (int m = 0; m < ROW_PAT; ++m) {
            int idx = (a << 6) | m;
            int cnt = 0;
            for (int n = 0; n < ROW_PAT; ++n) {
                if (row_valid((uint8_t)a, (uint8_t)m, (uint8_t)n, true, true)) {
                    middle_next_list[idx][cnt++] = (uint8_t)n;
                }
            }
            middle_next_count[idx] = (uint8_t)cnt;
        }
    }

    /* Last‑row validation (above exists, below does not) */
    for (int a = 0; a < ROW_PAT; ++a) {
        for (int m = 0; m < ROW_PAT; ++m) {
            int idx = (a << 6) | m;
            last_valid[idx] = row_valid((uint8_t)a, (uint8_t)m, 0, true, false);
        }
    }
}

/*====================================================================*/
/*  Main sequence function                                           */
/*====================================================================*/
seq_t a(idx_t n)
{
    init_precomp();

    /* Special case: single row */
    if (n == 0) return 0; /* not used, offset is 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int p0 = 0; p0 < ROW_PAT; ++p0) {
            if (p0 & 1) continue;                /* upper‑left must be 0 */
            if (row_valid(0, (uint8_t)p0, 0, false, false))
                ans += 1;
        }
        return ans;
    }

    /* Allocate DP arrays */
    seq_t *dp_curr = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(STATE_CNT * sizeof(seq_t));
    if (!dp_curr || !dp_next) return 0; /* allocation failure */

    memset(dp_curr, 0, STATE_CNT * sizeof(seq_t));

    /* Initialise with the first two rows (row 0 and row 1) */
    for (int p0 = 0; p0 < ROW_PAT; ++p0) {
        if (p0 & 1) continue;              /* upper‑left must be 0 */
        uint8_t *list = top_next_list[p0];
        int cnt = top_next_count[p0];
        for (int k = 0; k < cnt; ++k) {
            int p1 = list[k];
            int idx = (p0 << 6) | p1;
            dp_curr[idx] = 1;
        }
    }

    /* Process remaining rows */
    for (idx_t r = 2; r < n; ++r) {
        memset(dp_next, 0, STATE_CNT * sizeof(seq_t));
        for (int idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_curr[idx];
            if (!val) continue;
            int prev = idx & 0x3F;
            uint8_t *list = middle_next_list[idx];
            int cnt = middle_next_count[idx];
            for (int k = 0; k < cnt; ++k) {
                int nxt = list[k];
                int new_idx = (prev << 6) | nxt;
                dp_next[new_idx] += val;
            }
        }
        /* swap dp_curr and dp_next */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    /* Final validation of the last row */
    seq_t ans = 0;
    for (int idx = 0; idx < STATE_CNT; ++idx) {
        if (!dp_curr[idx]) continue;
        if (last_valid[idx]) ans += dp_curr[idx];
    }

    free(dp_curr);
    free(dp_next);
    return ans;
}
