/*
 * A269825  Number of nX4 0..4 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling four exactly once.
 * https://oeis.org/A269825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Result: a(n) = number of n×4 arrays over {0,…,4} with exactly one
 * pair of horizontally, vertically, diagonally or antidiagonally adjacent
 * cells whose entries sum to 4.  Computed by a transfer‑matrix DP with
 * 5⁴ = 625 row states and two layers for the quantity of pairs seen
 * (0 or 1).  The DP propagates row by row while keeping the pair
 * count bounded by 1.
 */

#include <stdbool.h>
#include <stdint.h>

/* Number of possible rows (5⁴). */
#define NUM_STATES 625
#define NUM_COLS   4

/* Lazy static initialization of tables. */
static bool tables_ready = false;
static unsigned char state_digits[NUM_STATES][NUM_COLS]; /* base‑5 digits 0‑4 */
static unsigned char h_pair[NUM_STATES];                /* horizontal pairs in a row */

/* Transition tables: for each previous state index, the start position
 * and length of the lists of next states that add 0 or 1 new sum‑4 pairs.
 */
static int    lists0_start[NUM_STATES];
static uint16_t lists0_len  [NUM_STATES];
static int    lists1_start[NUM_STATES];
static uint16_t lists1_len  [NUM_STATES];

/* Exactly one entry for each state, storing the next state index
 * (0‑624). Using uint16_t keeps the array compact.  The two arrays
 * share the same size because each state may have up to NUM_STATES
 * possible successors.
 */
static uint16_t lists0_all[NUM_STATES * NUM_STATES];
static uint16_t lists1_all[NUM_STATES * NUM_STATES];

/* One‑time initialization of state representations and
 * transition tables. */
static void init_tables(void)
{
    if (tables_ready) return;

    /* Pre‑compute the base‑5 digits for each state and
     * the number of horizontal pairs summing to 4 in that state. */
    for (int s = 0; s < NUM_STATES; ++s) {
        int tmp = s;
        for (int d = NUM_COLS - 1; d >= 0; --d) {
            state_digits[s][d] = tmp % 5;
            tmp /= 5;
        }
        unsigned char h = 0;
        for (int col = 0; col < NUM_COLS - 1; ++col)
            if (state_digits[s][col] + state_digits[s][col + 1] == 4)
                ++h;
        h_pair[s] = h;
    }

    /* Build transition lists. */
    int idx0 = 0, idx1 = 0;
    for (int prev = 0; prev < NUM_STATES; ++prev) {
        int start0 = idx0;
        uint16_t len0 = 0;
        int start1 = idx1;
        uint16_t len1 = 0;

        for (int next = 0; next < NUM_STATES; ++next) {
            /* Vertical and diagonal adjacency between prev and next. */
            unsigned char v = 0;
            for (int col = 0; col < NUM_COLS; ++col)
                if (state_digits[prev][col] + state_digits[next][col] == 4)
                    ++v;
            for (int col = 1; col < NUM_COLS; ++col)
                if (state_digits[prev][col] + state_digits[next][col - 1] == 4)
                    ++v;
            for (int col = 0; col < NUM_COLS - 1; ++col)
                if (state_digits[prev][col] + state_digits[next][col + 1] == 4)
                    ++v;

            unsigned char d = h_pair[next] + v;
            if (d == 0) {
                lists0_all[idx0++] = (uint16_t)next;
                ++len0;
            } else if (d == 1) {
                lists1_all[idx1++] = (uint16_t)next;
                ++len1;
            }
            /* Transitions with d > 1 are discarded. */
        }
        lists0_start[prev] = start0;
        lists0_len  [prev] = len0;
        lists1_start[prev] = start1;
        lists1_len  [prev] = len1;
    }

    tables_ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the offset */

    init_tables();

    /* DP arrays: dp0 = number of ways ending in a given state with
     * 0 pairs seen so far; dp1 = with exactly 1 pair seen. */
    static seq_t dp0[NUM_STATES];
    static seq_t dp1[NUM_STATES];
    static seq_t nxt0[NUM_STATES];
    static seq_t nxt1[NUM_STATES];

    /* Initialize for the first row. */
    for (int s = 0; s < NUM_STATES; ++s) {
        dp0[s] = 0;
        dp1[s] = 0;
    }
    for (int s = 0; s < NUM_STATES; ++s) {
        if (h_pair[s] == 0) dp0[s] = 1;
        else if (h_pair[s] == 1) dp1[s] = 1;
        /* States with >1 horizontal pairs are impossible. */
    }

    /* If only one row, sum the states with exactly one pair. */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < NUM_STATES; ++s) total += dp1[s];
        return total;
    }

    /* Process rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero next arrays. */
        for (int s = 0; s < NUM_STATES; ++s) {
            nxt0[s] = 0;
            nxt1[s] = 0;
        }

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t val0 = dp0[s];
            seq_t val1 = dp1[s];

            if (val0) {
                /* Transitions adding 0 new pairs keep pair count 0. */
                int      seg0   = lists0_start[s];
                uint16_t cnt0   = lists0_len  [s];
                uint16_t* next  = (uint16_t*)&lists0_all[seg0];
                for (uint16_t k = 0; k < cnt0; ++k)
                    nxt0[next[k]] += val0;

                /* Transitions adding 1 new pair move to pair count 1. */
                seg0   = lists1_start[s];
                cnt0   = lists1_len  [s];
                next   = (uint16_t*)&lists1_all[seg0];
                for (uint16_t k = 0; k < cnt0; ++k)
                    nxt1[next[k]] += val0;
            }

            if (val1) {
                /* Only 0‑new‑pair transitions keep the pair count at 1. */
                int      seg0   = lists0_start[s];
                uint16_t cnt0   = lists0_len  [s];
                uint16_t* next  = (uint16_t*)&lists0_all[seg0];
                for (uint16_t k = 0; k < cnt0; ++k)
                    nxt1[next[k]] += val1;
            }
        }

        /* Copy next arrays back to dp arrays for next iteration. */
        for (int s = 0; s < NUM_STATES; ++s) {
            dp0[s] = nxt0[s];
            dp1[s] = nxt1[s];
        }
    }

    /* Sum over states with exactly one qualifying pair. */
    seq_t result = 0;
    for (int s = 0; s < NUM_STATES; ++s) result += dp1[s];
    return result;
}
