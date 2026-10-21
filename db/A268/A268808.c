/*
 * A268808  Number of nX7 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A268808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 arrays over {0,1,2} in which at most one
 *            adjacent pair (horizontal, vertical or anti‑diagonal) sums to 2
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   clear_seq_arr, cross_sum, horizontal_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over rows.  Each state is a 7‑tuple of digits.
   Only states with at most one horizontal pair are kept; transitions between states
   are pre‑computed when the cross sum (vertical + antidiagonal) is at most one.
   The DP keeps two counters: whether zero or one pair has been used so far. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/clear_seq_arr.h"
#include "../utils/cross_sum.h"
#include "../utils/horizontal_sum.h"

#define MAX_STATES 2187                    /* 3^7 */

static int state_to_idx[MAX_STATES];      /* mapping from global state to compact idx (-1 if forbidden) */
static int idx_to_state[MAX_STATES];      /* inverse mapping: compact idx -> global state */
static uint8_t horiz_sum[MAX_STATES];     /* horizontal pair count for each global state (0 or 1) */
static uint8_t digits[MAX_STATES][7];     /* digit representation of each global state */
static int  S = 0;                       /* number of allowed states */

/* adjacency data structures */
static int start_edges[MAX_STATES + 1];           /* start index in edges array for each state */
static uint16_t edges_to[MAX_STATES * MAX_STATES];/* compact destination state */
static uint8_t  edges_cross[MAX_STATES * MAX_STATES]; /* vertical+antidiagonal pair count (0 or 1) */
static int edges_cnt = 0;

/* DP buffers (static to avoid dynamic allocation) */
static seq_t dp0_arr[MAX_STATES];
static seq_t dp1_arr[MAX_STATES];
static seq_t next0_arr[MAX_STATES];
static seq_t next1_arr[MAX_STATES];

static bool initialized = false;

/* ---------- helper functions -------------------------------------------------- */

/* ---------- initialization ---------------------------------------------------- */

static void init_tables(void)
{
    /* enumerate all 3^7 states */
    for (int s = 0; s < MAX_STATES; ++s) {
        int v = s;
        uint8_t h = 0;
        for (int col = 0; col < 7; ++col) {
            uint8_t d = v % 3;
            v /= 3;
            digits[s][col] = d;
            if (col > 0 && (digits[s][col - 1] + d == 2))
                h++;
        }
        horiz_sum[s] = (h <= 1) ? h : 255;        /* 255 marks forbidden state */
        if (h <= 1) {
            state_to_idx[s] = S;
            idx_to_state[S] = s;
            ++S;
        } else {
            state_to_idx[s] = -1;
        }
    }

    /* first pass: count outgoing edges from each state */
    static int count_out[MAX_STATES];
    for (int ai = 0; ai < S; ++ai) {
        int ai_orig = idx_to_state[ai];
        int cnt = 0;
        for (int aj = 0; aj < S; ++aj) {
            int aj_orig = idx_to_state[aj];
            if (cross_sum(digits[ai_orig], digits[aj_orig]) <= 1)
                ++cnt;
        }
        count_out[ai] = cnt;
    }

    /* build prefix sums for start indices */
    start_edges[0] = 0;
    for (int i = 0; i < S; ++i) {
        start_edges[i + 1] = start_edges[i] + count_out[i];
    }

    /* second pass: populate edges */
    for (int ai = 0; ai < S; ++ai) {
        int ai_orig = idx_to_state[ai];
        int pos = start_edges[ai];
        for (int aj = 0; aj < S; ++aj) {
            int aj_orig = idx_to_state[aj];
            uint8_t cs = cross_sum(digits[ai_orig], digits[aj_orig]);
            if (cs <= 1) {
                edges_to[pos]    = (uint16_t)aj;
                edges_cross[pos] = cs;
                ++pos;
            }
        }
    }

    edges_cnt = start_edges[S];
    initialized = true;
}

/* ---------- main sequence function -------------------------------------------- */

seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    /* DP pointers */
    seq_t *dp0 = dp0_arr;
    seq_t *dp1 = dp1_arr;
    seq_t *next0 = next0_arr;
    seq_t *next1 = next1_arr;

    /* clear DP arrays */
    clear_seq_arr(dp0, S);
    clear_seq_arr(dp1, S);

    /* initial row */
    for (int ai = 0; ai < S; ++ai) {
        uint8_t h = horiz_sum[idx_to_state[ai]];
        if (h == 0)
            dp0[ai] = 1;
        else /* h == 1 */
            dp1[ai] = 1;
    }

    /* iterative DP for subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next arrays */
        clear_seq_arr(next0, S);
        clear_seq_arr(next1, S);

        for (int ai = 0; ai < S; ++ai) {
            seq_t val0 = dp0[ai];
            seq_t val1 = dp1[ai];
            if (val0 == 0 && val1 == 0) continue;

            for (int e = start_edges[ai]; e < start_edges[ai + 1]; ++e) {
                int aj = edges_to[e];
                uint8_t cs = edges_cross[e];
                uint8_t hj = horiz_sum[idx_to_state[aj]];
                uint8_t tot = cs + hj;

                if (tot == 0) {
                    next0[aj] += val0;
                    if (val1 != 0)
                        next1[aj] += val1;
                } else if (tot == 1) {
                    next1[aj] += val0;   /* val1 cannot proceed with another pair */
                }
            }
        }

        /* swap dp and next buffers */
        seq_t *tmp;
        tmp = dp0; dp0 = next0; next0 = tmp;
        tmp = dp1; dp1 = next1; next1 = tmp;
    }

    /* sum all valid final states */
    seq_t result = 0;
    for (int i = 0; i < S; ++i) {
        result += dp0[i] + dp1[i];
    }
    return result;
}
