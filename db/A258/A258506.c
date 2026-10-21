/*
 * A258506  Number of (n+2)X(3+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the diagonal and antidiagonal minus the sum of
 *          the medians of the central row and column nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A258506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A258506 implementation: DP transfer‑matrix over rows
   of a (n+2)×5 binary array.
   See the analysis for details. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Problem parameters */
#define ROWS        32          /* number of 5‑bit rows 0 … 31            */
#define MONCOUNT    35          /* #non‑decreasing 3‑tuples (-2..2)          */
#define STATE_COUNT (ROWS * ROWS * MONCOUNT)   /* 35 840 */

/* Global tables (built lazily) */
static bool tables_initialized = false;

/* Mapping from packed 3‑tuple to index in val_vals[]; -1 if not monotonic */
static int8_t  code_to_idx[125];

/* The actual monotone vectors, indexed by 0 … MONCOUNT-1 */
static int8_t  val_vals[MONCOUNT][3];

/* For every ordered triple of rows (a,b,c) [] gives the vector index or -1 */
static int8_t  triple_idx[ROWS][ROWS][ROWS];

/* Edge tables for DP transitions */
static int   *edges_start;      /* start index in edges_flat for state i */
static int   *edges_len;        /* number of edges from state i */
static int   *edges_flat;       /* concatenated destination states */

/* Helper to initialise all tables (executed once, on first call) */
static void init_tables(void)
{
    /* 1. Build mapping of monotone 3‑tuples */
    int idx = 0;
    for (int code = 0; code < 125; ++code) {
        int v0 =  code / 25   - 2;
        int v1 = (code / 5)  % 5 - 2;
        int v2 =  code % 5   - 2;
        if (v0 <= v1 && v1 <= v2) {
            code_to_idx[code] = (int8_t)idx;
            val_vals[idx][0] = (int8_t)v0;
            val_vals[idx][1] = (int8_t)v1;
            val_vals[idx][2] = (int8_t)v2;
            ++idx;
        } else {
            code_to_idx[code] = -1;
        }
    }

    /* 2. Compute triple_idx for all row combinations */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            for (int c = 0; c < ROWS; ++c) {
                int v[3];
                for (int j = 0; j < 3; ++j) {
                    int diag_sum  = ((a >> j)       & 1) +
                                    ((b >> (j + 1)) & 1) +
                                    ((c >> (j + 2)) & 1);
                    int anti_sum  = ((a >> (j + 2)) & 1) +
                                    ((b >> (j + 1)) & 1) +
                                    ((c >> j)       & 1);
                    int row_sum   = ((b >> j)       & 1) +
                                    ((b >> (j + 1)) & 1) +
                                    ((b >> (j + 2)) & 1);
                    int col_sum   = ((a >> (j + 1)) & 1) +
                                    ((b >> (j + 1)) & 1) +
                                    ((c >> (j + 1)) & 1);
                    int mdiag     = diag_sum  >= 2;
                    int mantig    = anti_sum  >= 2;
                    int mrow      = row_sum   >= 2;
                    int mcol      = col_sum   >= 2;
                    v[j] = mdiag + mantig - mrow - mcol;   /* -2 … 2   */
                }
                int code = (v[0] + 2) * 25 + (v[1] + 2) * 5 + (v[2] + 2);
                triple_idx[a][b][c] = (int8_t)code_to_idx[code];
            }
        }
    }

    /* 3. Build adjacency lists for DP transitions */
    edges_start = (int *)malloc(STATE_COUNT * sizeof(int));
    edges_len   = (int *)malloc(STATE_COUNT * sizeof(int));
    /* maximum possible edges: each state can go to at most 32 new rows */
    edges_flat = (int *)malloc(STATE_COUNT * 32 * sizeof(int));

    int edge_used = 0;
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            for (int prev_i = 0; prev_i < MONCOUNT; ++prev_i) {
                int s_idx = ((a * ROWS) + b) * MONCOUNT + prev_i;
                edges_start[s_idx] = edge_used;
                int8_t *prev_v = val_vals[prev_i];
                for (int c = 0; c < ROWS; ++c) {
                    int new_v_idx = triple_idx[a][b][c];
                    if (new_v_idx < 0) continue;
                    int8_t *new_v = val_vals[new_v_idx];
                    /* vertical monotonicity */
                    bool ok = true;
                    for (int j = 0; j < 3; ++j) {
                        if (prev_v[j] > new_v[j]) { ok = false; break; }
                    }
                    /* antidiagonal monotonicity */
                    if (ok && (prev_v[1] > new_v[0] || prev_v[2] > new_v[1]))
                        ok = false;
                    if (!ok) continue;
                    int next_state = ((b * ROWS) + c) * MONCOUNT + new_v_idx;
                    edges_flat[edge_used++] = next_state;
                }
                edges_len[s_idx] = edge_used - edges_start[s_idx];
            }
        }
    }
    tables_initialized = true;
}

/* Core sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!tables_initialized)
        init_tables();

    /* Scratch buffers (lazily allocated once) */
    static seq_t *cur = NULL;
    static seq_t *nxt = NULL;
    if (!cur) {
        cur = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        nxt = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    }

    /* reset buffers */
    memset(cur, 0, STATE_COUNT * sizeof(seq_t));

    /* ---- initial step: first triple of rows (rows 0,1,2) ---- */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            for (int r2 = 0; r2 < ROWS; ++r2) {
                int v_idx = triple_idx[r0][r1][r2];
                if (v_idx < 0) continue;                 /* vector not sorted */
                int state = ((r1 * ROWS) + r2) * MONCOUNT + v_idx;  /* a=r1, b=r2 */
                ++cur[state];
            }
        }
    }

    /* ---- add remaining rows (n‑1 transitions) ---- */
    for (idx_t step = 1; step < n; ++step) {
        memset(nxt, 0, STATE_COUNT * sizeof(seq_t));
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t count = cur[s];
            if (count == 0) continue;
            int start = edges_start[s];
            int len   = edges_len[s];
            for (int e = 0; e < len; ++e) {
                int nxt_state = edges_flat[start + e];
                nxt[nxt_state] += count;
            }
        }
        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* ---- final answer: sum over all terminal states ---- */
    seq_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        total += cur[s];

    return total;
}
