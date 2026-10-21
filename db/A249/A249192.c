/*
 * A249192  Number of length n+6 0..3 arrays with no seven consecutive terms
 *          having four times the sum of any three elements equal to three
 *          times the sum of the remaining four.
 * https://oeis.org/A249192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 28, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 0..3 arrays of length n+6 in which no
 * seven consecutive terms have four times the sum of any three elements
 * equal to three times the sum of the remaining four.
 *
 * Computation: dynamic programming over the last six digits.  All
 * 6‑digit suffixes are states; each step appends a new digit and
 * checks the resulting 7‑digit window for the forbidden condition.
 * The transition table is built once and then reused for all calls.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* memset */

/* Compile‑time constants */
#define STATES 4096  /* 4^6 possible 6‑digit suffixes */
#define TRANS  4     /* four possible new digits (0..3) */
#define EXT    16384 /* 4^7 possible 7‑digit windows */

/* Transition table: next_state[s][v] = state after appending v to state s;
 * -1 means that the 7‑digit window is forbidden. */
static int16_t next_state[STATES][TRANS];

/* Flag indicating whether the transition table has been built. */
static bool transitions_built = false;

/* One‑time construction of the transition table. */
static void build_transitions(void)
{
    if (transitions_built)
        return;
    transitions_built = true;

    /* Precompute validity of all 7‑digit windows. */
    bool valid7[EXT];
    for (int code = 0; code < EXT; ++code) {
        /* Decode the 7 digits of the window. */
        int d[7];
        int temp = code;
        for (int i = 6; i >= 0; --i) {
            d[i] = temp & 3;     /* 0..3 */
            temp >>= 2;
        }
        int total = d[0] + d[1] + d[2] + d[3] + d[4] + d[5] + d[6];
        bool ok = true;
        /* Check all subsets of 3 indices. */
        for (int mask = 0; mask < 128; ++mask) {
            if (__builtin_popcount(mask) == 3) {
                int sum3 = 0;
                for (int i = 0; i < 7; ++i)
                    if (mask & (1 << i))
                        sum3 += d[i];
                int sum4 = total - sum3;
                if ((seq_t)4 * sum3 == (seq_t)3 * sum4) {
                    ok = false;
                    break;
                }
            }
        }
        valid7[code] = ok;
    }

    /* Build the transition table. */
    for (int s = 0; s < STATES; ++s) {
        for (int v = 0; v < TRANS; ++v) {
            int code7 = s * TRANS + v;          /* 7‑digit window code */
            if (!valid7[code7]) {
                next_state[s][v] = -1;
            } else {
                /* New state: drop the most significant digit and shift left. */
                int rest = s % 1024;   /* 4^5 = 1024 */
                int new_state = rest * TRANS + v;
                next_state[s][v] = (int16_t)new_state;
            }
        }
    }
}

/* Dynamic programming arrays are allocated once and reused. */
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;
static size_t dp_cap  = 0;

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* undefined for n < 1 */

    build_transitions();

    /* Ensure the DP arrays are allocated. */
    if (dp_cap < (size_t)STATES) {
        if (dp_curr) free(dp_curr);
        if (dp_next) free(dp_next);
        dp_curr = (seq_t *)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));
        dp_cap  = STATES;
    }

    /* Total length of the array to generate. */
    idx_t L = n + 6;

    /* Initialize DP for length 6: every 6‑digit pattern is valid. */
    for (int i = 0; i < STATES; ++i)
        dp_curr[i] = 1;

    /* Extend from length 7 up to L. */
    for (idx_t len = 7; len <= L; ++len) {
        /* Zero dp_next. */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        /* Transition from each state. */
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = dp_curr[s];
            if (cnt == 0)
                continue;
            for (int v = 0; v < TRANS; ++v) {
                int16_t ns = next_state[s][v];
                if (ns >= 0)
                    dp_next[ns] += cnt;
            }
        }

        /* Swap dp_curr and dp_next for the next iteration. */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    /* Sum all counts for the final length. */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += dp_curr[i];

    return total;
}
