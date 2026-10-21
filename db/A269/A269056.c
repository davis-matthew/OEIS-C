/*
 * A269056  Number of 5Xn 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of 5×n arrays with entries 0‑2 that contain at most one
 *            pair of horizontally, diagonally or antidiagonally adjacent
 *            cells whose values sum to 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 3^5 possible column patterns. */
#define STATE_COUNT 243
/* Upper bound for all zero or one‑pair transitions. */
#define MAX_TRANSITIONS (STATE_COUNT * STATE_COUNT)

/* Transition tables – built once. */
static bool built = false;
static int digits[STATE_COUNT][5];

static size_t zero_start[STATE_COUNT + 1];
static size_t one_start[STATE_COUNT + 1];
static uint16_t zero_adj[MAX_TRANSITIONS];
static uint16_t one_adj[MAX_TRANSITIONS];

/* Build the digit vectors and transition lists. */
static void build_tables(void)
{
    if (built) return;

    /* Convert each state to its 5‑digit representation in base 3. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int t = s;
        for (int r = 0; r < 5; ++r) {
            digits[s][r] = t % 3;
            t /= 3;
        }
    }

    /* Count how many zero‑ and one‑pair transitions each source has. */
    size_t zero_cnt[STATE_COUNT] = {0};
    size_t one_cnt[STATE_COUNT]  = {0};

    for (int i = 0; i < STATE_COUNT; ++i) {
        for (int j = 0; j < STATE_COUNT; ++j) {
            int inc = 0;
            for (int r = 0; r < 5; ++r) {
                if (digits[i][r] + digits[j][r] == 2) ++inc;
                if (r < 4) {
                    if (digits[i][r] + digits[j][r + 1] == 2) ++inc;
                    if (digits[i][r + 1] + digits[j][r] == 2) ++inc;
                }
            }
            if (inc == 0) ++zero_cnt[i];
            else if (inc == 1) ++one_cnt[i];
        }
    }

    /* Build prefix sums for indices in zero_adj/one_adj. */
    zero_start[0] = 0;
    one_start[0]  = 0;
    for (int i = 0; i < STATE_COUNT; ++i) {
        zero_start[i + 1] = zero_start[i] + zero_cnt[i];
        one_start[i + 1]  = one_start[i]  + one_cnt[i];
    }

    /* Temporary working arrays to keep the prefix sums intact. */
    size_t zpos[STATE_COUNT];
    size_t opos[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) {
        zpos[i] = zero_start[i];
        opos[i] = one_start[i];
    }

    /* Fill transition tables. */
    for (int i = 0; i < STATE_COUNT; ++i) {
        for (int j = 0; j < STATE_COUNT; ++j) {
            int inc = 0;
            for (int r = 0; r < 5; ++r) {
                if (digits[i][r] + digits[j][r] == 2) ++inc;
                if (r < 4) {
                    if (digits[i][r] + digits[j][r + 1] == 2) ++inc;
                    if (digits[i][r + 1] + digits[j][r] == 2) ++inc;
                }
            }
            if (inc == 0)
                zero_adj[zpos[i]++] = (uint16_t)j;
            else if (inc == 1)
                one_adj[opos[i]++]   = (uint16_t)j;
        }
    }

    built = true;
}

/* DP over columns.  Only two layers are needed. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    build_tables();

    static seq_t dp0[STATE_COUNT];
    static seq_t dp1[STATE_COUNT];
    static seq_t ndp0[STATE_COUNT];
    static seq_t ndp1[STATE_COUNT];

    /* First column – no pairs yet. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        dp0[s] = (seq_t)1;
        dp1[s] = (seq_t)0;
    }

    /* Process successive columns. */
    for (idx_t col = 2; col <= n; ++col) {
        /* Zero new tables. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            ndp0[s] = (seq_t)0;
            ndp1[s] = (seq_t)0;
        }

        for (int i = 0; i < STATE_COUNT; ++i) {
            seq_t val0 = dp0[i];
            seq_t val1 = dp1[i];

            if (val0) {
                /* Zero‑pair transitions keep state 0. */
                for (size_t p = zero_start[i]; p < zero_start[i + 1]; ++p) {
                    int w = zero_adj[p];
                    ndp0[w] = ndp0[w] + val0;
                }
                /* One‑pair transitions move to state 1. */
                for (size_t p = one_start[i]; p < one_start[i + 1]; ++p) {
                    int w = one_adj[p];
                    ndp1[w] = ndp1[w] + val0;
                }
            }
            if (val1) {
                /* From state 1 only zero‑pair transitions are allowed. */
                for (size_t p = zero_start[i]; p < zero_start[i + 1]; ++p) {
                    int w = zero_adj[p];
                    ndp1[w] = ndp1[w] + val1;
                }
            }
        }

        /* Copy new tables back. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            dp0[s] = ndp0[s];
            dp1[s] = ndp1[s];
        }
    }

    /* Sum all admissible endings. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        total = total + dp0[s] + dp1[s];
    }
    return total;
}
