/*
 * A253734  Number of (n+2)X(2+2) 0..1 arrays with every 2X2 and 3X3 subblock
 *          diagonal sum minus antidiagonal sum unequal to its neighbors
 *          horizontally and vertically.
 * https://oeis.org/A253734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 42, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Count 0/1 arrays of size (n+2) × 4 (4 columns) satisfying:
   • For every 2×2 block, (sum of main diagonal) – (sum of anti‑diagonal)
     is unequal to the corresponding value of its horizontal and vertical
     neighboring 2×2 blocks.
   • For every 3×3 block the same property holds.
   This is done by dynamic programming over the last three rows of the
   array.  The function is self‑contained and automatically initialises
   the lookup tables on the first call.
*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------- constants for state representation ---------- */
#define ROWS 4          /* number of bits in a row (columns of the array) */
#define STATES 4096     /* 16³ possible triples of rows */

/* ---------- lookup tables ---------- */
static bool  pair_h_valid[16][16];
static int8_t pair_d[16][16][3];

static bool  triple_h_valid[16][16][16];
static int8_t triple_d[16][16][16][2];

static uint8_t    trans_cnt[STATES];
static uint16_t  trans_next[STATES][16];

/* ---------- table initialisation ---------- */
static void init_tables(void)
{
    static bool constructed = false;
    if (constructed) return;
    constructed = true;

    /* compute pair tables */
    for (uint8_t r1 = 0; r1 < 16; ++r1) {
        for (uint8_t r2 = 0; r2 < 16; ++r2) {
            for (int j = 0; j < 3; ++j) {
                int diag = ((r1 >> j)      & 1) +
                           ((r2 >> (j + 1)) & 1);
                int anti = ((r1 >> (j + 1)) & 1) +
                           ((r2 >> j)       & 1);
                pair_d[r1][r2][j] = (int8_t)(diag - anti);
            }
            /* horizontal uniqueness of the two 2×2 blocks in the pair */
            pair_h_valid[r1][r2] =
                pair_d[r1][r2][0] != pair_d[r1][r2][1] &&
                pair_d[r1][r2][1] != pair_d[r1][r2][2];
        }
    }

    /* compute triple tables (only two starting positions for a 3×3 block) */
    for (uint8_t a = 0; a < 16; ++a) {
        for (uint8_t b = 0; b < 16; ++b) {
            for (uint8_t c = 0; c < 16; ++c) {
                for (int j = 0; j < 2; ++j) {
                    int diag = ((a >> j)       & 1) +
                               ((b >> (j + 1)) & 1) +
                               ((c >> (j + 2)) & 1);
                    int anti = ((a >> (j + 2)) & 1) +
                               ((b >> (j + 1)) & 1) +
                               ((c >> j)       & 1);
                    triple_d[a][b][c][j] = (int8_t)(diag - anti);
                }
                /* horizontal uniqueness of the two 3×3 blocks in the triple */
                triple_h_valid[a][b][c] =
                    triple_d[a][b][c][0] != triple_d[a][b][c][1];
            }
        }
    }

    /* pre‑compute admissible transitions */
    for (uint16_t id = 0; id < STATES; ++id) {
        uint8_t a = (uint8_t)(id >> 8);
        uint8_t b = (uint8_t)((id >> 4) & 0xF);
        uint8_t c = (uint8_t)(id & 0xF);
        uint8_t cnt = 0;
        for (uint8_t d = 0; d < 16; ++d) {
            /* horizontal uniqueness for the new pair (c,d)            */
            if (!pair_h_valid[c][d]) continue;
            /* vertical uniqueness of 2×2 blocks between (b,c) and (c,d) */
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                if (pair_d[b][c][j] == pair_d[c][d][j]) { ok = false; break; }
            }
            if (!ok) continue;
            /* horizontal uniqueness for the new triple (b,c,d)        */
            if (!triple_h_valid[b][c][d]) continue;
            /* vertical uniqueness of 3×3 blocks between (a,b,c) and (b,c,d) */
            for (int j = 0; j < 2; ++j) {
                if (triple_d[a][b][c][j] == triple_d[b][c][d][j]) {
                    ok = false; break;
                }
            }
            if (!ok) continue;
            trans_next[id][cnt++] = (uint16_t)((b << 8) | (c << 4) | d);
        }
        trans_cnt[id] = cnt;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for offset 1 */

    init_tables();

    /* DP buffers */
    static seq_t dp_cur[STATES];
    static seq_t dp_nxt[STATES];
    seq_t *cur = dp_cur, *nxt = dp_nxt;

    /* initialise with all valid 3‑row states (first 3 rows) */
    memset(dp_cur, 0, sizeof(dp_cur));

    for (uint8_t a = 0; a < 16; ++a)
        for (uint8_t b = 0; b < 16; ++b)
            for (uint8_t c = 0; c < 16; ++c) {
                if (!pair_h_valid[a][b]) continue;
                if (!pair_h_valid[b][c]) continue;
                if (!triple_h_valid[a][b][c]) continue;
                /* vertical uniqueness of 2×2 blocks between (a,b) and (b,c) */
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    if (pair_d[a][b][j] == pair_d[b][c][j]) { ok = false; break; }
                }
                if (!ok) continue;
                uint16_t id = (uint16_t)((a << 8) | (b << 4) | c);
                cur[id] = (seq_t)1;
            }

    /* number of additional rows to add */
    idx_t steps = n - 1;                    /* (n+2)-3 = n-1 */
    for (idx_t step = 0; step < steps; ++step) {
        memset(nxt, 0, sizeof(dp_nxt));
        for (uint16_t id = 0; id < STATES; ++id) {
            seq_t cnt = cur[id];
            if (!cnt) continue;
            uint8_t t = trans_cnt[id];
            for (uint8_t k = 0; k < t; ++k) {
                uint16_t next_id = trans_next[id][k];
                nxt[next_id] += cnt;
            }
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum all terminal states */
    seq_t res = (seq_t)0;
    for (uint16_t id = 0; id < STATES; ++id) res += cur[id];
    return res;
}
