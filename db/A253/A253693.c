/*
 * A253693  Number of (n+1)X(3+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference unequal to its neighbors horizontally and
 *          nw+se diagonal sum unequal to its neighbors vertically.
 * https://oeis.org/A253693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1) x 4 binary arrays in which for every 2×2
 *            subblock the NE–SW difference is not equal to its horizontal
 *            neighbour and the NW–SE diagonal sum is not equal to its
 *            vertical neighbour. Computed by dynamic programming over row
 *            patterns. a(1)=104, a(2)=150, …
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 65, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool  pair_ok[16][16];
static unsigned short s_code[16][16];
static unsigned char digits[27][3];
static bool init_done = false;

static void init_tables(void) {
    if (init_done) return;
    for (int code = 0; code < 27; ++code) {
        int t = code;
        digits[code][0] = (unsigned char)(t % 3); t /= 3;
        digits[code][1] = (unsigned char)(t % 3); t /= 3;
        digits[code][2] = (unsigned char)(t % 3);
    }

    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            /* sums of NW–SE diagonals for the three 2×2 subblocks */
            int s[3];
            for (int j = 0; j < 3; ++j) {
                int bit0 = (r0 >> j)      & 1;
                int bit1 = (r1 >> (j+1))  & 1;
                s[j] = bit0 + bit1;      /* 0,1,2 */
            }
            s_code[r0][r1] = (unsigned short)(s[0] + 3*s[1] + 9*s[2]);

            /* horizontal antidiagonal differences */
            int d[3];
            for (int j = 0; j < 3; ++j) {
                int ne = (r0 >> (j+1)) & 1;  /* north‑east */
                int sw = (r1 >>  j   ) & 1;  /* south‑west */
                d[j] = ne - sw;              /* -1,0,1 */
            }
            pair_ok[r0][r1] = (d[0] != d[1]) && (d[1] != d[2]);
        }
    }
    init_done = true;
}

seq_t a(idx_t n) {
    init_tables();

    /* DP arrays – they persist across calls but are zeroed each invocation */
    static seq_t dp0[16 * 27];
    static seq_t dp1[16 * 27];
    seq_t *dp_cur  = dp0;
    seq_t *dp_next = dp1;

    memset(dp_cur, 0, sizeof(dp0));

    /* first pair of rows (rows 0 and 1) */
    for (int r0 = 0; r0 < 16; ++r0) {
        for (int r1 = 0; r1 < 16; ++r1) {
            if (!pair_ok[r0][r1]) continue;
            int sc = s_code[r0][r1];
            dp_cur[r1 * 27 + sc] += (seq_t)1;
        }
    }

    if (n == 0) return (seq_t)0;     /* offset is 1 – guard against out of range */

    if (n == 1) {                    /* two rows only */
        seq_t sum = 0;
        for (size_t i = 0; i < sizeof(dp0)/sizeof(dp0[0]); ++i)
            sum += dp_cur[i];
        return sum;
    }

    /* append rows 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp1));

        for (int prev_r = 0; prev_r < 16; ++prev_r) {
            for (int prev_s = 0; prev_s < 27; ++prev_s) {
                seq_t val = dp_cur[prev_r * 27 + prev_s];
                if (val == (seq_t)0) continue;

                for (int next_r = 0; next_r < 16; ++next_r) {
                    if (!pair_ok[prev_r][next_r]) continue;

                    int new_s = s_code[prev_r][next_r];

                    /* vertical constraint: NW–SE sums must differ for each column */
                    bool ok = true;
                    for (int k = 0; k < 3; ++k) {
                        if (digits[prev_s][k] == digits[new_s][k]) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;

                    dp_next[next_r * 27 + new_s] += val;
                }
            }
        }

        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states */
    seq_t result = 0;
    for (size_t i = 0; i < sizeof(dp0)/sizeof(dp0[0]); ++i)
        result += dp_cur[i];
    return result;
}
