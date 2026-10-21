/*
 * A247924  Number of length n+4 0..5 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247924: number of length n+4 0..5 arrays such that in every
   block of five consecutive terms there exist two disjoint pairs
   with equal sum.  Computed by dynamic programming over the last
   four terms. */

#include <stdint.h>
#include <stdbool.h>

#define STATES (1296)         /* 6^4 states for the last four digits */

static bool tables_initialized = false;

/* Transition from a state (last four digits) by appending a new digit v */
static uint16_t next_state[STATES][6];
/* Whether the 5‑tuple (state digits + v) contains two disjoint equal‑sum pairs */
static unsigned char valid_tbl[STATES][6];

static void init_tables(void)
{
    if (tables_initialized) return;
    for (int s = 0; s < STATES; ++s) {
        int k1 = s / 216;
        int rem = s % 216;
        int k2 = rem / 36;
        rem %= 36;
        int k3 = rem / 6;
        int k4 = rem % 6;

        for (int v = 0; v < 6; ++v) {
            /* compute new state after shifting left and appending v */
            int ns = ((k2 * 6 + k3) * 6 + k4) * 6 + v;
            next_state[s][v] = (uint16_t)ns;

            /* test the 5‑tuple for the required property */
            int a[5] = {k1, k2, k3, k4, v};
            int sum[10];
            unsigned short mask[10];
            int pcnt = 0;
            for (int i = 0; i < 5; ++i) {
                for (int j = i + 1; j < 5; ++j) {
                    sum[pcnt] = a[i] + a[j];
                    mask[pcnt] = (1U << i) | (1U << j);
                    ++pcnt;
                }
            }

            bool ok = false;
            for (int p = 0; p < pcnt && !ok; ++p) {
                for (int q = p + 1; q < pcnt; ++q) {
                    if (sum[p] == sum[q] && (mask[p] & mask[q]) == 0) {
                        ok = true;
                        break;
                    }
                }
            }
            valid_tbl[s][v] = ok ? 1 : 0;
        }
    }
    tables_initialized = true;
}

seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays for the current and next length */
    static seq_t dp_curr[STATES];
    static seq_t dp_next[STATES];

    /* Initial length 4: all 4‑tuples are allowed */
    for (int s = 0; s < STATES; ++s) dp_curr[s] = 1;

    /* Extend by n new digits to reach length n+4 */
    for (idx_t step = 1; step <= n; ++step) {
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cur = dp_curr[s];
            if (!cur) continue;
            for (int v = 0; v < 6; ++v) {
                if (valid_tbl[s][v]) {
                    int ns = next_state[s][v];
                    dp_next[ns] += cur;
                }
            }
        }
        /* swap dp arrays */
        for (int s = 0; s < STATES; ++s) dp_curr[s] = dp_next[s];
    }

    /* Sum over all ending states to obtain the total count */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s) ans += dp_curr[s];
    return ans;
}
