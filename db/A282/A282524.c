/*
 * A282524  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors.
 * https://oeis.org/A282524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×4 binary arrays where every 1 has at
 *            most * two king‑move neighbors. Computed using a deterministic
 *            dynamic * programming algorithm with a 3‑row memory window
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool ok_table[16][16][16];
static bool prepared = false;

static void compute_ok_table(void) {
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if (((b >> j) & 1) == 0) continue;
                    int cnt = 0;
                    for (int di = -1; di <= 1 && ok; ++di) {
                        for (int dj = -1; dj <= 1 && ok; ++dj) {
                            if (di == 0 && dj == 0) continue;
                            int col = j + dj;
                            if (col < 0 || col > 3) continue;
                            int mask = 1 << col;
                            int bits;
                            if (di == -1) bits = a;
                            else if (di == 0) bits = b;
                            else bits = c;
                            if (bits & mask) ++cnt;
                        }
                    }
                    if (cnt > 2) ok = false;
                }
                ok_table[a][b][c] = ok;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!prepared) {
        compute_ok_table();
        prepared = true;
    }

    /* DP arrays: dp[cur_state] holds the number of ways up to the current
     * row where 'prev' and 'curr' are the last two rows processed. */
    seq_t dp_cur[256] = {0};
    seq_t dp_next[256] = {0};

    /* Initial state: row 1 follows an empty top row (all zeros). */
    for (int r1 = 0; r1 < 16; ++r1)
        dp_cur[r1] = 1; /* state (prev=0, curr=r1) */

    /* Iterate from row 2 up to n */
    for (int row = 2; row <= (int)n; ++row) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp_cur[prev * 16 + curr];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (!ok_table[prev][curr][nxt]) continue;
                    dp_next[curr * 16 + nxt] += ways;
                }
            }
        }
        /* swap current and next */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final summation: ensure the last row satisfies constraints with
     * an empty bottom row (all zeros). */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (!ok_table[prev][curr][0]) continue;
            result += dp_cur[prev * 16 + curr];
        }
    }

    return result;
}
