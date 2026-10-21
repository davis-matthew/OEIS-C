/*
 * A250077  Number of length n+5 0..4 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 31, u128 n <= 70,
 *            big any n
 * Helpers:   is_6tuple_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A250077 (OEIS).  Uses a DP over 5‑digit states
   to count length n+5 arrays of digits 0..4 where every 6‑consecutive
   window satisfies:  max(one two of the window) = min of the other
   four.  The DP enumerates all 5‑digit suffixes; at each step we
   append a new digit, check the resulting 6‑tuple, and accumulate
   counts.  No hard‑coded values are used.  */

#include <stdbool.h>
#include "../utils/is_6tuple_valid.h"

#define STATES 3125        /* 5^5 */

static unsigned char state_digits[STATES][5];   /* digits of each 5‑tuple */
static unsigned int  next_state[STATES][5];     /* resulting state after append */
static bool          valid_tr[STATES][5];      /* whether 6‑tuple is legal */
static unsigned int  pow5[6] = {1,5,25,125,625,3125};

static bool init_done = false;

/* Initialise transition tables on first call */
static void
init_transitions(void)
{
    if (init_done) return;
    unsigned int s, v, i;

    /* Pre‑compute the digits for every 5‑tuple */
    for (s = 0; s < STATES; ++s) {
        unsigned int tmp = s;
        for (int pos = 4; pos >= 0; --pos) {
            state_digits[s][pos] = tmp % 5;
            tmp /= 5;
        }
    }

    /* Build transitions and validity flags */
    unsigned char six[6];
    for (s = 0; s < STATES; ++s) {
        for (v = 0; v < 5; ++v) {
            unsigned int ns = ((s % pow5[4]) * 5 + v);
            next_state[s][v] = ns;

            for (i = 0; i < 5; ++i) six[i] = state_digits[s][i];
            six[5] = (unsigned char)v;
            valid_tr[s][v] = is_6tuple_valid(six);
        }
    }
    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) corresponds to array length 6 (n=1) */
    if (n == 0) return (seq_t)0;

    init_transitions();

    /* Two DP arrays */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    unsigned int i, step, max_step = (unsigned int)n;

    /* initial DP: all 5‑tuples are possible before the first 6‑window */
    for (i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;

    for (step = 0; step < max_step; ++step) {
        /* zero next array */
        for (i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        for (i = 0; i < STATES; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            for (unsigned int v = 0; v < 5; ++v) {
                if (!valid_tr[i][v]) continue;
                unsigned int ns = next_state[i][v];
                dp_next[ns] += val;
            }
        }
        /* swap dp_cur and dp_next by copying */
        for (i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* total count after processing all steps */
    seq_t result = (seq_t)0;
    for (i = 0; i < STATES; ++i) result += dp_cur[i];

    return result;
}
