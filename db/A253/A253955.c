/*
 * A253955  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          median minus antidiagonal median nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   diff_for_subblock
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by dynamic programming over all valid 3‑row
   triples for 4‑column binary arrays with the nondecreasing
   diagonal‑minus‑antidiagonal median constraint.  No closed‑form
   formula is known. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/diff_for_subblock.h"

 /* The build supplies idx_t and seq_t.  Do not #define them here. */

static int built = 0;                 /* graph built flag */
static size_t num_states = 0;          /* number of valid triples */

/* Max number of states (16³).  Arrays are indexed by 0 … num_states-1. */
#define MAX_STATES 4096

/* State representation: the three rows of a 3‑row block.  Each row
   fits in 4 bits (0..15). */
static uint16_t state_a[MAX_STATES];
static uint16_t state_b[MAX_STATES];
static uint16_t state_c[MAX_STATES];
static int8_t  state_d0[MAX_STATES];  /* diff for subblock at column 0 */
static int8_t  state_d1[MAX_STATES];  /* diff for subblock at column 1 */

/* Transition graph.  Every state may have up to 16 outgoing edges. */
static int trans[MAX_STATES][16];
static uint8_t trans_cnt[MAX_STATES];

/* Mapping from a triple (a,b,c) to its state index, -1 if not valid. */
static int16_t triple_to_index[16][16][16];

/* Dynamic programming arrays.  We keep them statically allocated so
   that the code works both in C (where calloc would zero‑initialize
   integers) and in C++ (where static arrays get default–constructed). */
static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];
static seq_t *dp_cur_ptr = dp_cur;
static seq_t *dp_next_ptr = dp_next;

/* Results cache: results[n] holds a(n) for 1 ≤ n ≤ MAX_RESULTS-1. */
#define MAX_RESULTS 100001   /* enough for n up to about 10000 */
static seq_t results[MAX_RESULTS];
static size_t cur_max_n = 0;   /* the largest n that has been computed */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Build the state graph once.  The graph is small (≤ 4096 states). */
static void build_graph(void)
{
    /* Initialise mapping to -1 */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 16; ++k)
                triple_to_index[i][j][k] = -1;

    num_states = 0;

    /* Enumerate all triples (a,b,c) and keep those whose internal
       horizontal diff is nondecreasing: d0 ≤ d1. */
    for (uint16_t a = 0; a < 16; ++a) {
        for (uint16_t b = 0; b < 16; ++b) {
            for (uint16_t c = 0; c < 16; ++c) {
                int8_t d0 = diff_for_subblock(a, b, c, 0);
                int8_t d1 = diff_for_subblock(a, b, c, 1);
                if (d0 <= d1) {
                    state_a[num_states] = a;
                    state_b[num_states] = b;
                    state_c[num_states] = c;
                    state_d0[num_states] = d0;
                    state_d1[num_states] = d1;
                    triple_to_index[a][b][c] = (int16_t)num_states;
                    ++num_states;
                }
            }
        }
    }

    /* Build transitions: (a,b,c) → (b,c,d) subject to vertical
       and anti‑diagonal nondecreasing constraints. */
    for (size_t i = 0; i < num_states; ++i)
        trans_cnt[i] = 0;

    for (size_t i = 0; i < num_states; ++i) {
        uint16_t a = state_a[i];
        uint16_t b = state_b[i];
        uint16_t c = state_c[i];
        for (uint16_t d = 0; d < 16; ++d) {
            int16_t j = triple_to_index[b][c][d];
            if (j < 0) continue;

            /* Vertical: d0[i] ≤ d0[j]  and  d1[i] ≤ d1[j]  (anti‑diagonal) */
            if (state_d0[i] <= state_d0[j] &&
                state_d1[i] <= state_d1[j] &&
                state_d1[i] <= state_d0[j]) {
                uint8_t k = trans_cnt[i]++;
                trans[i][k] = j;
            }
        }
    }

    built = 1;
}

/* ------------------------------------------------------------ */
/* The main sequence function.  It can be called for any n ≥ 1 in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range: not needed for offset 1 */
    if (!built) build_graph();

    size_t N = (size_t)n;
    if (N >= MAX_RESULTS) return 0;    /* safety: we only support up to MAX_RESULTS-1 */

    /* Compute results up to N if we haven't already. */
    if (N <= cur_max_n) return results[N];

    /* Initialise DP for n = 1 if this is the first call. */
    if (cur_max_n == 0) {
        for (size_t i = 0; i < num_states; ++i)
            dp_cur_ptr[i] = 1;           /* each triple forms a 3×4 array */
        results[1] = (seq_t)num_states;   /* sum of dp_cur_ptr over all states */
        results[1] = (seq_t)num_states;
        cur_max_n = 1;
    }

    /* Extend the DP to steps cur_max_n+1 … N. */
    for (size_t step = cur_max_n + 1; step <= N; ++step) {
        /* dp_next holds a fresh array; zero it. */
        for (size_t i = 0; i < num_states; ++i)
            dp_next_ptr[i] = 0;

        /* Transition from dp_cur_ptr to dp_next_ptr. */
        for (size_t i = 0; i < num_states; ++i) {
            seq_t val = dp_cur_ptr[i];
            if (val == 0) continue;
            uint8_t deg = trans_cnt[i];
            for (uint8_t k = 0; k < deg; ++k) {
                int j = trans[i][k];
                dp_next_ptr[j] += val;
            }
        }

        /* Swap dp arrays for the next iteration. */
        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_next_ptr;
        dp_next_ptr = tmp;

        /* Sum over all states to obtain a(step). */
        seq_t total = 0;
        for (size_t i = 0; i < num_states; ++i)
            total += dp_cur_ptr[i];
        results[step] = total;
    }

    cur_max_n = N;
    return results[N];
}
