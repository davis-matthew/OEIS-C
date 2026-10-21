/*
 * A253758  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          median minus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×(2+2) 0/1 arrays whose 3×3 block
 *            diagonal‑median minus anti‑diagonal‑median differences are
 *            horizontal and vertical non‑decreasing
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   decode, majority3, state_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/decode.h"
#include "../utils/majority3.h"
#include "../utils/state_idx.h"

/* ---- Precomputation of median differences for a 3×3 block ---- */

static signed char d0_tab[16][16][16];   /* j = 0  (main diagonal) */
static signed char d1_tab[16][16][16];   /* j = 1  (anti‑diagonal) */
static bool tables_ready = false;

/* Build the tables only once */
static void precompute_tables(void)
{
    if (tables_ready)
        return;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2) {
                /* j = 0 */
                int m0 = majority3((r0 >> 0) & 1, (r1 >> 1) & 1,
                                   (r2 >> 2) & 1);
                int a0 = majority3((r0 >> 2) & 1, (r1 >> 1) & 1,
                                   (r2 >> 0) & 1);
                d0_tab[r0][r1][r2] = (signed char)(m0 - a0);  /* -1,0,1 */

                /* j = 1 */
                int m1 = majority3((r0 >> 1) & 1, (r1 >> 2) & 1,
                                   (r2 >> 3) & 1);
                int a1 = majority3((r0 >> 3) & 1, (r1 >> 2) & 1,
                                   (r2 >> 1) & 1);
                d1_tab[r0][r1][r2] = (signed char)(m1 - a1);
            }
    tables_ready = true;
}

/* Helpers to map d ∈ {‑1,0,1} to an index 0,1,2 */
static inline int encode(int d) { return d + 1; }

/* The 9 possible (prev0,prev1) pairs encoded as a single index.   */
static const signed char pd0[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
static const signed char pd1[9] = { -1,  0,  1, -1, 0, 1, -1, 0, 1 };

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0.     */
    if (n == 0)
        return (seq_t)0;

    precompute_tables();

    const int ROWS   = 16;         /* 0 … 15 */
    const int DENS   = 9;          /* 3×3 previous‑difference pairs */
    const int STATES = ROWS * ROWS * DENS;   /* 2304 */

    /* Allocate the DP buffers only once.  Cast to seq_t* so the code
       also compiles in C++ (a void * cast is required there).    */
    static seq_t *dp     = NULL;
    static seq_t *dp_next = NULL;
    if (!dp) {
        dp      = (seq_t *)calloc(STATES, sizeof(seq_t));
        dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    }

    /* ---- Initialise with the first three rows ----- */
    memset(dp, 0, STATES * sizeof(seq_t));
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2) {
                int d0 = d0_tab[r0][r1][r2];
                int d1 = d1_tab[r0][r1][r2];
                if (d1 < d0)                 /* horizontal non‑decreasing */
                    continue;
                int p0 = encode(d0);
                int p1 = encode(d1);
                int idx = state_idx(r1, r2, p0, p1);
                dp[idx] += (seq_t)1;
            }

    /* ---- Propagate through the remaining (n−1) rows ----- */
    for (idx_t step = 1; step <= n - 1; ++step) {
        memset(dp_next, 0, STATES * sizeof(seq_t));
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
                for (int prev = 0; prev < 9; ++prev) {
                    int idx = state_idx(r1, r2, prev / 3, prev % 3);
                    seq_t cur = dp[idx];
                    if (!cur)
                        continue;
                    int prev_d0 = pd0[prev];
                    int prev_d1 = pd1[prev];
                    for (int r3 = 0; r3 < 16; ++r3) {
                        int d0 = d0_tab[r1][r2][r3];
                        int d1 = d1_tab[r1][r2][r3];
                        if (d1 < d0)                     /* horizontal */
                            continue;
                        if (d0 < prev_d0 || d1 < prev_d1)/* vertical   */
                            continue;
                        int p0 = encode(d0);
                        int p1 = encode(d1);
                        int next_idx = state_idx(r2, r3, p0, p1);
                        dp_next[next_idx] += cur;
                    }
                }
        /* swap dp and dp_next for the next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* ---- Sum all final states to obtain the result ---- */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i)
        result += dp[i];

    return result;
}
