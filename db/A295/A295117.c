/*
 * A295117  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 4 1s.
 * https://oeis.org/A295117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which every 1 has exactly
 *            * 1 or 4 horizontally/vertically adjacent 1s
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Constants for the transfer‑matrix */
#define ROWS       5
#define MASKS      (1 << ROWS)     /* 32 */
#define STATE_SZ   (MASKS * MASKS) /* 1024 */

/* Precomputed tables (initialised once) */
static bool _init_done = false;
static uint32_t _trans[STATE_SZ];          /* bitmask of allowed next masks */
static uint8_t  _vdeg[MASKS][ROWS];      /* vertical degree for each bit in a mask */

/* Initialise the transition table and vertical degree table.
 * This is performed on first entry into a() */
static void _init(void) {
    if (_init_done) return;
    /* Precompute vertical degrees (number of vertical neighbours within column) */
    for (int mask = 0; mask < MASKS; ++mask) {
        for (int r = 0; r < ROWS; ++r) {
            if ((mask >> r) & 1) {
                int d = 0;
                if (r > 0 && ((mask >> (r - 1)) & 1)) ++d;
                if (r < ROWS - 1 && ((mask >> (r + 1)) & 1)) ++d;
                _vdeg[mask][r] = (uint8_t)d;
            } else {
                _vdeg[mask][r] = 0;
            }
        }
    }
    /* Precompute transition table */
    for (int prev = 0; prev < MASKS; ++prev) {
        for (int cur = 0; cur < MASKS; ++cur) {
            uint32_t bits = 0;
            for (int next = 0; next < MASKS; ++next) {
                bool ok = true;
                for (int r = 0; r < ROWS; ++r) {
                    if ((cur >> r) & 1) {
                        int deg = _vdeg[cur][r]
                            + ((prev >> r) & 1)
                            + ((next >> r) & 1);
                        if (deg != 1 && deg != 4) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) bits |= (1U << next);
            }
            _trans[prev * MASKS + cur] = bits;
        }
    }
    _init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined below offset, return 0 */

    _init();               /* ensure tables are ready */

    /* Two DP arrays, toggled between columns */
    static seq_t _dp[2][STATE_SZ];
    seq_t *cur = _dp[0];
    seq_t *nxt = _dp[1];

    /* Zero initialise */
    memset(cur, 0, sizeof(seq_t) * STATE_SZ);
    memset(nxt, 0, sizeof(seq_t) * STATE_SZ);

    /* First column: left boundary is zero */
    for (int mask = 0; mask < MASKS; ++mask) {
        cur[mask] = (seq_t)1;   /* state (prev=0, cur=mask) */
    }

    /* Process remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next array */
        memset(nxt, 0, sizeof(seq_t) * STATE_SZ);

        for (int idx = 0; idx < STATE_SZ; ++idx) {
            seq_t cnt = cur[idx];
            if (cnt == (seq_t)0) continue;

            uint32_t bits = _trans[idx];
            /* iterate set bits in bits */
            while (bits) {
                int next_mask = __builtin_ctz(bits);  /* position of lowest set bit */
                bits &= bits - 1;                    /* clear that bit */
                int cur_mask = idx & (MASKS - 1);
                int new_idx = cur_mask * MASKS + next_mask;
                nxt[new_idx] = nxt[new_idx] + cnt;
            }
        }
        /* swap pointers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Final column: right boundary is zero */
    seq_t result = 0;
    for (int idx = 0; idx < STATE_SZ; ++idx) {
        seq_t cnt = cur[idx];
        if (cnt == (seq_t)0) continue;

        int prev_mask = idx / MASKS;
        int cur_mask  = idx % MASKS;
        bool ok = true;

        for (int r = 0; r < ROWS; ++r) {
            if ((cur_mask >> r) & 1) {
                int deg = _vdeg[cur_mask][r] + ((prev_mask >> r) & 1); /* right edge is 0 */
                if (deg != 1 && deg != 4) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) result = result + cnt;
    }

    return result;
}
