/*
 * A282993  Number of nX5 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A282993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays with no 1 having more than one
 *            neighboring 1 among its horizontal and vertical neighbors.
 *            Computation uses a DP over rows, storing for each column the
 *            state of a 1 cell: 0 = 0, 1 = 1 with zero neighboring 1's so
 *            far, 2 = 1 with one neighboring 1's so far. The tran...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required headers for integer types and bool. */
#include <stdint.h>
#include <stdbool.h>

/* Transition table: from a previous state (243 possibilities)
   and a current row bit mask (32 possibilities) to a next state
   (243 possibilities) or -1 if the transition is invalid. */
static int16_t trans[243][32];
static bool trans_inited = false;

/* Helper arrays for transition precomputation. */
static uint8_t pow3[5] = {1, 3, 9, 27, 81};
static uint8_t digits[243][5];

/* Precompute the transition table once. */
static void init_transitions(void) {
    /* Compute the base‑3 digit representation for every state. */
    for (int s = 0; s < 243; ++s) {
        int x = s;
        for (int j = 0; j < 5; ++j) {
            digits[s][j] = x % 3;
            x /= 3;
        }
    }

    /* For each previous state and each possible row mask, build the
       transition. */
    for (int s = 0; s < 243; ++s) {
        uint8_t *prev = digits[s];
        for (int mask = 0; mask < 32; ++mask) {
            /* 1. Check that a 1 cell that already has one neighbor
               in the vertical direction does not get an extra 1
               below. */
            bool bad = false;
            for (int j = 0; j < 5 && !bad; ++j) {
                if (prev[j] == 2 && (mask & (1 << j)))
                    bad = true;
            }
            if (bad) {
                trans[s][mask] = -1;
                continue;
            }

            /* 2. Compute the annotation for the current row. */
            uint8_t next[5];
            bool invalid = false;
            for (int j = 0; j < 5; ++j) {
                if (!(mask & (1 << j))) {          /* cell is 0 */
                    next[j] = 0;
                    continue;
                }
                int up    = (prev[j] != 0) ? 1 : 0;
                int left  = (j > 0 && (mask & (1 << (j - 1)))) ? 1 : 0;
                int right = (j < 4 && (mask & (1 << (j + 1)))) ? 1 : 0;
                int tot = up + left + right;
                if (tot > 1) {                     /* would exceed limit */
                    invalid = true;
                    break;
                }
                next[j] = (tot == 1) ? 2 : 1;       /* 1 with 1 or 0 neighbors */
            }
            if (invalid) {
                trans[s][mask] = -1;
                continue;
            }

            /* Encode next state into an integer 0..242. */
            int idx = 0;
            for (int j = 0; j < 5; ++j)
                idx += next[j] * pow3[j];
            trans[s][mask] = (int16_t)idx;
        }
    }
    trans_inited = true;
}

/* Main function requested by the build system. */
seq_t a(idx_t n) {
    /* Return 0 for n==0 (undefined by OEIS offset). */
    if (n == 0)
        return 0;

    if (!trans_inited)
        init_transitions();

    /* DP arrays: number of ways to reach each state after processing i rows. */
    static seq_t dp[243];
    static seq_t ndp[243];

    /* Initialize DP for zero rows: only the zero state is reachable. */
    for (int i = 0; i < 243; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Process each row. */
    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < 243; ++i)
            ndp[i] = 0;

        for (int s = 0; s < 243; ++s) {
            if (dp[s] == 0)
                continue;
            for (int mask = 0; mask < 32; ++mask) {
                int ns = trans[s][mask];
                if (ns < 0)
                    continue;
                ndp[ns] += dp[s];
            }
        }
        for (int i = 0; i < 243; ++i)
            dp[i] = ndp[i];
    }

    /* Sum over all final states; each is a valid completion of n rows. */
    seq_t result = 0;
    for (int i = 0; i < 243; ++i)
        result += dp[i];
    return result;
}
