/*
 * A253759  Number of (n+2)X(3+2) 0..1 arrays with every 3X3 subblock diagonal
 *          median minus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253759
 * Number of (n+2)×(5) binary arrays with every 3×3 subblock
 * (diagonal median – antidiagonal median) nondecreasing horizontally
 * and vertically.
 * The implementation uses a dynamic‑programming over rows with a
 * precomputed transition table. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- configuration constants ---------- */
#define ROW_BITS   5               /* 5 columns → 5 bits per row   */
#define ROW_MASK   0x1F            /* mask for a row (5 bits)      */
#define PAIR_SIZE  1024            /* 32 × 32 row pairs            */
#define DENS       11              /* 10 valid D‑encodings + 1 undef */
#define STATES     (PAIR_SIZE*DENS)/* total DP states */
#define UNDEF_IDX  10              /* index for undefined D value  */

/* ---------- transition structure ---------- */
typedef struct {
    uint8_t newRow;   /* next row pattern (0..31)      */
    uint8_t dEnc;     /* encoded difference vector (0..9) */
} TransInfo;

/* ---------- global tables ---------- */
static bool        vertOk[DENS][DENS];
static int         dEncIdx[3][3][3];
static int         dValues[DENS-1][3];  /* only 0..9 used */
static TransInfo   trans_all[PAIR_SIZE*32]; /* worst‑case 32 per pair */
static int         trans_start[PAIR_SIZE];
static int         trans_count[PAIR_SIZE];
static bool        tables_ready = false;

/* ---------- helper: encode DP state index ---------- */
static inline int state_index(int a, int b, int dPrevIdx)
{
    return ((a << ROW_BITS) | b) * DENS + dPrevIdx;
}

/* ---------- precompute transition and vertical tables ---------- */
static void precompute_tables(void)
{
    /* ---- generate mapping for difference vectors ---- */
    int idx = 0;
    for (int a = -1; a <= 1; ++a)
        for (int b = a;     b <= 1; ++b)
            for (int c = b;     c <= 1; ++c) {
                dValues[idx][0] = a;
                dValues[idx][1] = b;
                dValues[idx][2] = c;
                dEncIdx[a+1][b+1][c+1] = idx;
                ++idx;
            }
    /* map UNDEF is not stored in tables; its index is UNDEF_IDX */

    /* ---- vertical monotonicity table ---- */
    for (int p = 0; p < DENS-1; ++p)
        for (int q = 0; q < DENS-1; ++q) {
            bool ok = true;
            for (int k = 0; k < 3; ++k)
                if (dValues[p][k] > dValues[q][k]) { ok = false; break; }
            vertOk[p][q] = ok;
        }

    /* ---- horizontal transitions for all (a,b) pairs ---- */
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            int pairIdx  = (a << ROW_BITS) | b;
            int pos = pairIdx * 32; /* start position in trans_all */
            trans_start[pairIdx] = pos;
            int count = 0;

            for (int c = 0; c < 32; ++c) {
                int d[3];
                /* compute D for each of the three columns of the 3×3 block */
                for (int col = 0; col < 3; ++col) {
                    int diag = ((a >> col)     & 1) +
                               ((b >> (col+1)) & 1) +
                               ((c >> (col+2)) & 1);
                    int anti = ((a >> (col+2)) & 1) +
                               ((b >> (col+1)) & 1) +
                               ((c >> col)     & 1);
                    d[col] = ((diag >= 2) ? 1 : 0) - ((anti >= 2) ? 1 : 0);
                }
                /* horizontal monotonicity check */
                if (d[0] <= d[1] && d[1] <= d[2]) {
                    int enc = dEncIdx[d[0]+1][d[1]+1][d[2]+1];
                    trans_all[pos + count].newRow = (uint8_t)c;
                    trans_all[pos + count].dEnc   = (uint8_t)enc;
                    ++count;
                }
            }
            trans_count[pairIdx] = count;
        }
    }
    tables_ready = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise tables once */
    if (!tables_ready)
        precompute_tables();

    /* DP arrays */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    for (int i = 0; i < STATES; ++i) cur[i] = nxt[i] = 0;

    /* initial state: choose any two rows; vertical monotonicity undefined */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            cur[state_index(a, b, UNDEF_IDX)] = 1;

    /* process additional rows (there are n additional rows after the first two) */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (int idx = 0; idx < STATES; ++idx) {
            seq_t val = cur[idx];
            if (!val) continue;

            int dPrevIdx = idx % DENS;
            int pairIdx  = idx / DENS;
            int a        = pairIdx >> ROW_BITS;
            int b        = pairIdx & ROW_MASK;

            int start = trans_start[pairIdx];
            int cnt   = trans_count[pairIdx];
            for (int t = 0; t < cnt; ++t) {
                TransInfo const *tr = &trans_all[start + t];
                if (dPrevIdx != UNDEF_IDX && !vertOk[dPrevIdx][tr->dEnc])
                    continue;
                int newIdx = state_index(b, tr->newRow, tr->dEnc);
                nxt[newIdx] += val;
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* sum all final states */
    seq_t ans = 0;
    for (int i = 0; i < STATES; ++i)
        ans += cur[i];
    return ans;
}
