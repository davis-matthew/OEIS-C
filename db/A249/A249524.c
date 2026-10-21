/*
 * A249524  Number of length n+5 0..2 arrays with no six consecutive terms
 *          having five times any element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of length n+5 0..2 arrays with no six consecutive
 *            terms having five times any element equal to the sum of the
 *            remaining five
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define STATE_COUNT 243     /* 3^5 possible tuples of last five terms */

static bool _init_done = false;
static int _state_sum5[STATE_COUNT];
static bool _state_has1[STATE_COUNT];
static bool _state_has2[STATE_COUNT];

/* Precompute state properties once */
static void _init_state(void)
{
    if (_init_done) return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int v = s;
        int sum = 0;
        bool h1 = false, h2 = false;
        for (int k = 0; k < 5; ++k) {
            int d = v % 3;
            v /= 3;
            sum += d;
            if (d == 1) h1 = true;
            if (d == 2) h2 = true;
        }
        _state_sum5[s]  = sum;
        _state_has1[s]  = h1;
        _state_has2[s]  = h2;
    }
    _init_done = true;
}

/* Dynamic programming arrays */
static seq_t _dp_cur[STATE_COUNT];
static seq_t _dp_next[STATE_COUNT];

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we return 0 for n==0. */
    if (n == 0) return 0;

    _init_state();

    idx_t length = n + 5;          /* total length of the array to be counted */

    /* Initialize DP for the first 5 positions (any 5-tuple is allowed) */
    for (int s = 0; s < STATE_COUNT; ++s)
        _dp_cur[s] = 1;

    /* Process positions 6 .. length */
    for (idx_t pos = 6; pos <= length; ++pos) {
        /* zero the next DP array */
        for (int s = 0; s < STATE_COUNT; ++s)
            _dp_next[s] = 0;

        /* transition from each state using possible next element y = 0,1,2 */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = _dp_cur[s];
            if (cur == 0) continue;          /* skip empty state */

            int sum5    = _state_sum5[s];
            bool has1_5 = _state_has1[s];
            bool has2_5 = _state_has2[s];

            for (int y = 0; y <= 2; ++y) {
                int sum6 = sum5 + y;
                bool has1 = has1_5 || (y == 1);
                bool has2 = has2_5 || (y == 2);

                /* Check forbidden window */
                bool bad = false;
                if (sum6 == 0)                     bad = true;            /* all zeros */
                else if (sum6 == 6 && has1)        bad = true;            /* value 1 present */
                else if (sum6 == 12 && has2)       bad = true;            /* value 2 present */
                if (bad) continue;                /* discard this extension */

                /* Compute new state: drop oldest value, keep last4, append y */
                int new_state = (s % 81) * 3 + y;     /* 81 == 3^4 */
                _dp_next[new_state] += cur;
            }
        }
        /* swap current and next arrays for next iteration */
        for (int s = 0; s < STATE_COUNT; ++s)
            _dp_cur[s] = _dp_next[s];
    }

    /* Sum all valid end states to get the final count */
    seq_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        total += _dp_cur[s];

    return total;
}
