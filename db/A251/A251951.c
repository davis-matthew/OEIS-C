/*
 * A251951  Number of (n+2)X(1+2) 0..2 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 3.
 * https://oeis.org/A251951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Computation by dynamic programming over admissible rows */

/* seq_t and idx_t are type aliases provided by the build system */

seq_t a(idx_t n)
{
    const idx_t L = n + 2;

    /* Allowed rows (those whose sum is not 3) */
    static uint8_t rows[20][3];
    static unsigned char R = 0;

    /* Transition table: from pair (prev two rows) to new pair (last two rows) */
    static int next_state[400][20];
    static unsigned char deg[400];
    static bool initialized = false;

    if (!initialized) {
        int idx = 0;
        for (int a = 0; a <= 2; ++a)
            for (int b = 0; b <= 2; ++b)
                for (int c = 0; c <= 2; ++c) {
                    int s = a + b + c;
                    if (s == 3) continue;            /* row sum constraint */
                    rows[idx][0] = (uint8_t)a;
                    rows[idx][1] = (uint8_t)b;
                    rows[idx][2] = (uint8_t)c;
                    ++idx;
                }
        R = (unsigned char)idx;                      /* should be 20 */

        /* Build the transition table */
        for (int a = 0; a < R; ++a)
            for (int b = 0; b < R; ++b) {
                int st = a * R + b;
                deg[st] = 0;
                for (int c = 0; c < R; ++c) {
                    bool ok = true;
                    /* column sums must not be 3 */
                    for (int k = 0; k < 3; ++k) {
                        if (rows[a][k] + rows[b][k] + rows[c][k] == 3) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;
                    /* main diagonal */
                    if (rows[a][0] + rows[b][1] + rows[c][2] == 3) continue;
                    /* anti-diagonal */
                    if (rows[a][2] + rows[b][1] + rows[c][0] == 3) continue;

                    int nxt = b * R + c;
                    next_state[st][deg[st]++] = nxt;
                }
            }
        initialized = true;
    }

    static seq_t dp_current[400];
    static seq_t dp_next[400];
    const int S = R * R;          /* number of state pairs */

    /* Initially any pair of allowed rows is acceptable */
    for (int i = 0; i < S; ++i)
        dp_current[i] = 1;

    idx_t steps = L - 2;          /* number of additional rows to add */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < S; ++i)
            dp_next[i] = 0;

        for (int st = 0; st < S; ++st) {
            seq_t val = dp_current[st];
            if (!val) continue;
            for (int k = 0; k < deg[st]; ++k) {
                int nxt = next_state[st][k];
                dp_next[nxt] += val;
            }
        }

        for (int i = 0; i < S; ++i)
            dp_current[i] = dp_next[i];
    }

    /* Sum over all final states to obtain the total count */
    seq_t result = 0;
    for (int i = 0; i < S; ++i)
        result += dp_current[i];

    return result;
}
