/*
 * A249465  Number of length n+4 0..7 arrays with no five consecutive terms
 *          having four times any element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts of 0..7 arrays of length n+4 with no five consecutive *
 *            terms having four times any element equal to the sum of the
 *            other four
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define STATE_COUNT 4096   /* 8^4 states */

static bool transition_ok[STATE_COUNT][8] = {0};
static bool transition_ok_inited = false;

static void init_transition_ok(void)
{
    if (transition_ok_inited) return;
    transition_ok_inited = true;

    for (int s = 0; s < STATE_COUNT; ++s) {
        int a = (s >> 9) & 7;
        int b = (s >> 6) & 7;
        int c = (s >> 3) & 7;
        int d =  s        & 7;
        for (int e = 0; e < 8; ++e) {
            int sum = a + b + c + d + e;
            bool bad = false;
            if (sum % 5 == 0) {
                int v = sum / 5;
                if (v == a || v == b || v == c || v == d || v == e)
                    bad = true;
            }
            transition_ok[s][e] = !bad;
        }
    }
}

static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];
static bool dp_init = false;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_transition_ok();

    /* Length = n + 4 */
    idx_t target_len = n + 4;

    /* Base: all 4‑tuples are allowed for length 4 */
    for (int s = 0; s < STATE_COUNT; ++s) dp_cur[s] = 1;

    /* Build up to target length */
    for (idx_t len = 5; len <= target_len; ++len) {
        /* clear next array */
        for (int s = 0; s < STATE_COUNT; ++s) dp_next[s] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            int b = (s >> 6) & 7;
            int c = (s >> 3) & 7;
            int d =  s        & 7;

            for (int e = 0; e < 8; ++e) {
                if (!transition_ok[s][e]) continue;

                int ns = (b << 9) | (c << 6) | (d << 3) | e;
                dp_next[ns] += val;
            }
        }
        /* copy next to cur for next iteration */
        for (int s = 0; s < STATE_COUNT; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all states for the final length */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) result += dp_cur[s];
    return result;
}
