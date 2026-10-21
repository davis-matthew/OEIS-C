/*
 * A296959  Number of nX4 0..1 arrays with each 1 adjacent to 2, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 4 0..1 arrays where each 1 has 2, 3, or 5
 *            king‑move neighboring 1s. The sequence satisfies a linear
 *            recurrence derived from a 16×16 transfer matrix; this program
 *            implements that transfer matrix directly
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool valid_triple[16][16][16] = {0};

static inline bool compute_valid(uint8_t a, uint8_t b, uint8_t c)
{
    for (int col = 0; col < 4; ++col) {
        if (!(b & (1 << col))) continue;
        int cnt = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            uint8_t rowbits;
            if (dr == -1) rowbits = a;
            else if (dr == 0) rowbits = b;
            else rowbits = c;
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = col + dc;
                if (nc < 0 || nc > 3) continue;
                if (rowbits & (1 << nc)) ++cnt;
            }
        }
        if (cnt != 2 && cnt != 3 && cnt != 5) return false;
    }
    return true;
}

static void init_valid_triple(void)
{
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c)
                valid_triple[a][b][c] = compute_valid((uint8_t)a, (uint8_t)b, (uint8_t)c);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static bool prepared = false;
    if (!prepared) {
        init_valid_triple();
        prepared = true;
    }

    seq_t dp[16][16] = {0};
    seq_t dp_next[16][16];

    /* Initialize with top row missing (all zeros) */
    for (int row0 = 0; row0 < 16; ++row0) {
        dp[0][row0] = 1;
    }

    /* Process additional rows */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp_next[i][j] = 0;

        for (int prevprev = 0; prevprev < 16; ++prevprev) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t val = dp[prevprev][prev];
                if (!val) continue;
                for (int curr = 0; curr < 16; ++curr) {
                    if (valid_triple[prevprev][prev][curr]) {
                        dp_next[prev][curr] += val;
                    }
                }
            }
        }

        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final check for the last row */
    seq_t total = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (valid_triple[prev][curr][0]) {
                total += dp[prev][curr];
            }
        }
    }

    return total;
}
