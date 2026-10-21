/*
 * A251331  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2, 4, or 6.
 * https://oeis.org/A251331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows; a(n) counts (n+1)×5 0..2 arrays whose every 2×2
 *            subblock sums to 2, 4, or 6. The routine builds a transfer graph
 *            on all 3^5 states and performs a row‑by‑row dynamic program
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Parameters of the DP */
#define WIDTH       5                /* number of columns */
#define STATE_COUNT (243)           /* 3^5 possible rows */
#define MAX_N       10000           /* maximum n we support           */

/* Transition adjacency lists: next_list[a][k] is the k‑th allowed next state. */
static int         next_list[STATE_COUNT][STATE_COUNT];
static unsigned char next_count[STATE_COUNT];

/* Base‑3 digits of each state (least significant first). */
static unsigned char state_digits[STATE_COUNT][WIDTH];

/* DP tables for the current and next row. */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* Memoised answers for all n ≤ MAX_N.  Static array is zero‑initialized. */
static seq_t answers[MAX_N + 1];
static size_t max_computed = 0;

/* Flag whether helper data are initialised. */
static bool initialised = false;

/* --------------------------------------------------------------------- */
/* Helper functions                                                      */
/* --------------------------------------------------------------------- */

/* Convert an integer state into its base‑3 digits. */
static void encode_state(unsigned int s, unsigned char *digits) {
    unsigned int t = s;
    for (size_t i = 0; i < WIDTH; ++i) {
        digits[i] = t % 3;
        t /= 3;
    }
}

/* Test whether transition from state a to state b satisfies all 2×2‐sum
   constraints. */
static bool transition_allowed(int a, int b) {
    for (size_t c = 0; c < WIDTH - 1; ++c) {
        int sum =
            state_digits[a][c] + state_digits[a][c + 1] +
            state_digits[b][c] + state_digits[b][c + 1];
        if (sum != 2 && sum != 4 && sum != 6) return false;
    }
    return true;
}

/* Initialise static tables on first use.  */
static void initialise(void) {
    if (initialised) return;
    initialised = true;

    /* Compute digits of each state. */
    for (int s = 0; s < STATE_COUNT; ++s)
        encode_state((unsigned int)s, state_digits[s]);

    /* Build adjacency lists. */
    for (int a = 0; a < STATE_COUNT; ++a) {
        next_count[a] = 0;
        for (int b = 0; b < STATE_COUNT; ++b)
            if (transition_allowed(a, b))
                next_list[a][next_count[a]++] = b;
    }

    /* Initialise dp_cur: every first row is allowed. */
    for (size_t i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = (seq_t)1;
}

/* --------------------------------------------------------------------- */
/* Compute answers up to the requested n.                               */
/* --------------------------------------------------------------------- */
static void compute_up_to(idx_t n) {
    initialise();

    if (n <= max_computed) return;
    if (n > MAX_N) {
        /* If the build expects larger n, extend by re‑allocating a larger
           static array or abort; here we simply stop at MAX_N. */
        /* In practice, n will not exceed MAX_N according to the constraints. */
        exit(1);
    }

    /* Perform DP steps from max_computed+1 up to n. */
    for (idx_t step = (idx_t)(max_computed + 1); step <= n; ++step) {
        /* Zero dp_next. */
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition from each state a to all allowed states b. */
        for (int a = 0; a < STATE_COUNT; ++a) {
            seq_t val = dp_cur[a];
            if (val == 0) continue;
            unsigned char cnt = next_count[a];
            for (unsigned char k = 0; k < cnt; ++k) {
                int b = next_list[a][k];
                dp_next[b] += val;
            }
        }

        /* Sum over all possible final states gives a(step). */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < STATE_COUNT; ++i)
            sum += dp_next[i];

        answers[(size_t)step] = sum;

        /* Prepare for next iteration. */
        for (size_t i = 0; i < STATE_COUNT; ++i)
            dp_cur[i] = dp_next[i];
    }

    max_computed = (size_t)n;
}

/* --------------------------------------------------------------------- */
/* Public API: a(n)                                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    compute_up_to(n);
    return answers[(size_t)n];
}
