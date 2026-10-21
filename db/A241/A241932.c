/*
 * A241932  Number of length n+4 0..4 arrays with no consecutive five elements
 *          summing to more than 2*4.
 * https://oeis.org/A241932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays with entries 0..4 such that
 *            no 5 consecutive elements sum to more than 8
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static uint8_t digit[625][4];
static uint8_t sum4[625];

static void init_tables(void) {
    for (int s = 0; s < 625; ++s) {
        int x = s;
        digit[s][3] = x % 5; x /= 5;
        digit[s][2] = x % 5; x /= 5;
        digit[s][1] = x % 5; x /= 5;
        digit[s][0] = x % 5;
        sum4[s] = digit[s][0] + digit[s][1] + digit[s][2] + digit[s][3];
    }
    initialized = true;
}

seq_t a(idx_t n) {
    /* n is 1-based index; the length of the arrays is N = n + 4 */
    idx_t N = n + 4;
    idx_t steps = N - 4; /* number of new elements to add after the first 4 */
    seq_t dp0[625];
    seq_t dp1[625];
    seq_t *cur = dp0;
    seq_t *nxt = dp1;

    if (!initialized) init_tables();

    /* initial state: all 4-tuples are allowed */
    for (int i = 0; i < 625; ++i) cur[i] = 1;

    for (idx_t step = 0; step < steps; ++step) {
        /* clear next array */
        for (int i = 0; i < 625; ++i) nxt[i] = 0;

        for (int s = 0; s < 625; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue; /* skip zero counts to save work */
            uint8_t ssum = sum4[s];
            for (uint8_t d = 0; d < 5; ++d) {
                if (ssum + d > 8) continue; /* window would exceed sum 8 */
                int newstate = ((digit[s][1] * 5 + digit[s][2]) * 5 + digit[s][3]) * 5 + d;
                nxt[newstate] += val;
            }
        }
        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* total number of arrays of length N is sum over all ending states */
    seq_t total = 0;
    for (int i = 0; i < 625; ++i) total += cur[i];
    return total;
}
