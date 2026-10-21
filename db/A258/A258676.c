/*
 * A258676  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum.
 * https://oeis.org/A258676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+2)×5 binary arrays in which, for every
 *            3×3 subblock, the sum of the main–diagonal elements is not equal
 *            to the sum of the anti‑diagonal elements. A DP over the rows is
 *            used; no closed‑form expression is known
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* There are 2^5 = 32 possible row patterns. */
#define ROWS 32
#define STATES (ROWS * ROWS)

/* Transition masks: allow[p][q] has bit r set iff a row r can follow rows
   p and q without violating the 3×3 rule. */
static uint32_t allow[ROWS][ROWS];
static bool allow_ready = false;

/* Compute the allow table once. */
static void compute_allow(void)
{
    for (int p = 0; p < ROWS; ++p) {
        for (int q = 0; q < ROWS; ++q) {
            uint32_t mask = 0;
            for (int r = 0; r < ROWS; ++r) {
                bool ok = true;
                /* For each of the 3 possible horizontal positions of a 3×3 block. */
                for (int j = 0; j < 3; ++j) {
                    int a = (p >> j)       & 1;   /* row p, column j            */
                    int b = (q >> (j + 1)) & 1;   /* row q, column j+1          */
                    int c = (r >> (j + 2)) & 1;   /* row r, column j+2          */
                    int d = (p >> (j + 2)) & 1;   /* anti‑diagonal start (row p)*/
                    int e = (q >> (j + 1)) & 1;   /* middle column (row q)      */
                    int f = (r >> j)       & 1;   /* anti‑diagonal end   (row r)*/
                    int diag  = a + b + c;        /* main diagonal sum          */
                    int anti  = d + e + f;        /* anti‑diagonal sum          */
                    if (diag == anti) { ok = false; break; }
                }
                if (ok) mask |= 1U << r;
            }
            allow[p][q] = mask;
        }
    }
    allow_ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* Sequence is defined starting at n = 1. */
        return (seq_t)0;
    }

    /* Number of rows in the matrix. */
    idx_t rows = n + 2;     /* n+2 rows, always 5 columns */

    /* Allocate DP arrays. */
    seq_t *dp_cur  = (seq_t*)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(STATES, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(dp_cur);
        free(dp_next);
        return (seq_t)0;
    }

    /* Every pair of the first two rows is initially allowed. */
    for (idx_t i = 0; i < STATES; ++i) {
        dp_cur[i] = (seq_t)1;
    }

    /* Build the transition mask once. */
    if (!allow_ready) {
        compute_allow();
    }

    /* Process each additional row beyond the first two. */
    for (idx_t step = 3; step <= rows; ++step) {
        /* Zero out dp_next. */
        for (idx_t i = 0; i < STATES; ++i) {
            dp_next[i] = (seq_t)0;
        }

        /* Transition from each state. */
        for (idx_t s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            int prevprev = (int)(s / ROWS);
            int prev     = (int)(s % ROWS);
            uint32_t mask = allow[prevprev][prev];
            while (mask) {
                int r = __builtin_ctz(mask);
                mask &= mask - 1;
                idx_t ns = (idx_t)prev * ROWS + r;
                dp_next[ns] += val;
            }
        }

        /* Swap dp_cur and dp_next for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all counts of final states. */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < STATES; ++i) {
        total += dp_cur[i];
    }

    /* Free allocated arrays. */
    free(dp_cur);
    free(dp_next);

    return total;
}
