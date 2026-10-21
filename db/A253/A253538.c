/*
 * A253538  Number of (n+2) X (2+2) 0..1 arrays with every 3 X 3 subblock
 *          diagonal maximum minus antidiagonal minimum nonincreasing
 *          horizontally and nondecreasing vertically.
 * https://oeis.org/A253538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253538: number of (n+2)×4 binary arrays such that for every 3×3
   subblock the quantity (maximum of the main diagonal entries) minus
   (minimum of the anti‑diagonal entries) is non‑increasing from left
   to right and non‑decreasing from top to bottom.
   The value is computed by dynamic programming over rows.
   State:  (rowk‑2 , rowk‑1 , diff0 , diff1)
   where rowk‑2 and rowk‑1 are the two most recent full rows (4 bits),
   diff0 and diff1 are the preceding subblock differences on the left
   (columns 0‑2) and right (columns 1‑3) side respectively.
   Only three difference values are possible: –1, 0, 1.   */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Number of possible 4‑bit rows. */
#define ROWS 16
/* Possible difference states: –1,0,1. */
#define T_STATES 3
/* Total number of DP states. */
#define MAX_STATE (ROWS * ROWS * T_STATES * T_STATES)

/* Convert difference –1,0,1 to index 0,1,2. */
#define T_IDX(v) ((int)((v) + 1))

/* Compute flattened state index from its components. */
#define STATE_IDX(a,b,tL,tR) ((((a) * ROWS + (b)) * T_STATES + T_IDX(tL)) * T_STATES + T_IDX(tR))

/* DP function. */
seq_t a(idx_t n)
{
    /* Precomputed difference values for every triple of rows. */
    static int8_t t_left[ROWS][ROWS][ROWS];
    static int8_t t_right[ROWS][ROWS][ROWS];
    static bool t_init = false;
    if (!t_init) {
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                for (int c = 0; c < ROWS; ++c) {
                    int diag  = ((a >> 0) & 1) | ((b >> 1) & 1) | ((c >> 2) & 1);
                    int ant   = ((a >> 2) & 1) & ((b >> 1) & 1) & ((c >> 0) & 1);
                    t_left[a][b][c] = (int8_t)(diag - ant);
                    diag = ((a >> 1) & 1) | ((b >> 2) & 1) | ((c >> 3) & 1);
                    ant  = ((a >> 3) & 1) & ((b >> 2) & 1) & ((c >> 1) & 1);
                    t_right[a][b][c] = (int8_t)(diag - ant);
                }
        t_init = true;
    }

    /* Decoded state fields for fast access. */
    static uint8_t  state_a[MAX_STATE];
    static uint8_t  state_b[MAX_STATE];
    static int8_t   state_tL[MAX_STATE];
    static int8_t   state_tR[MAX_STATE];
    static bool state_populated = false;
    if (!state_populated) {
        int idx = 0;
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                for (int tL = 0; tL < T_STATES; ++tL)
                    for (int tR = 0; tR < T_STATES; ++tR) {
                        state_a[idx]  = (uint8_t)a;
                        state_b[idx]  = (uint8_t)b;
                        state_tL[idx] = (int8_t)(tL - 1);
                        state_tR[idx] = (int8_t)(tR - 1);
                        ++idx;
                    }
        state_populated = true;
    }

    /* Precompute all valid transitions. */
    static int trans[MAX_STATE][ROWS];
    static bool trans_populated = false;
    if (!trans_populated) {
        for (int s = 0; s < MAX_STATE; ++s) {
            uint8_t a = state_a[s];
            uint8_t b = state_b[s];
            int8_t tL = state_tL[s];
            int8_t tR = state_tR[s];
            for (int newRow = 0; newRow < ROWS; ++newRow) {
                int8_t tLnext = t_left[a][b][newRow];
                int8_t tRnext = t_right[a][b][newRow];
                /* constraints: horizontal non‑increasing, vertical non‑decreasing */
                if (tL <= tLnext && tR <= tRnext && tLnext >= tRnext) {
                    int nextIdx = STATE_IDX(b, newRow, tLnext, tRnext);
                    trans[s][newRow] = nextIdx;
                } else {
                    trans[s][newRow] = -1;  /* invalid transition */
                }
            }
        }
        trans_populated = true;
    }

    /* DP arrays for current and next step. */
    static seq_t cur[MAX_STATE];
    static seq_t nxt[MAX_STATE];

    /* Clear current DP array. */
    for (int i = 0; i < MAX_STATE; ++i) cur[i] = 0;

    /* Initialize: first subblock (rows 0,1,2). */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            for (int c = 0; c < ROWS; ++c) {
                int8_t tL = t_left[a][b][c];
                int8_t tR = t_right[a][b][c];
                if (tL >= tR) {
                    int idx = STATE_IDX(b, c, tL, tR);
                    cur[idx] += (seq_t)1;
                }
            }

    /* If n == 1 we have only one subblock. */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < MAX_STATE; ++i) total += cur[i];
        return total;
    }

    /* Process subsequent subblocks. */
    for (idx_t step = 0; step < n - 1; ++step) {
        for (int i = 0; i < MAX_STATE; ++i) nxt[i] = 0;
        for (int s = 0; s < MAX_STATE; ++s) {
            seq_t v = cur[s];
            if (v == 0) continue;
            for (int newRow = 0; newRow < ROWS; ++newRow) {
                int next = trans[s][newRow];
                if (next >= 0) nxt[next] += v;
            }
        }
        for (int i = 0; i < MAX_STATE; ++i) cur[i] = nxt[i];
    }

    /* Sum all valid final states. */
    seq_t result = 0;
    for (int i = 0; i < MAX_STATE; ++i) result += cur[i];
    return result;
}
