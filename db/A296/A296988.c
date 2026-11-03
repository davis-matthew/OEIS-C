/*
 * A296988  Number of nX6 0..1 arrays with each 1 adjacent to 3, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 180 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A296988 –  
   Number of n×6 0..1 arrays where each 1 has 3,4 or 5 king‑move neighbors.
   The algorithm uses a transfer‑matrix dynamic programming over row pairs.
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Checks if a given row (mask) satisfies the neighbor condition
   when the rows above (=above) and below (=below) are known. */
static inline bool validate_row(uint8_t row, uint8_t above, uint8_t below)
{
    for (int col = 0; col < 6; ++col) {
        if (row & (1u << col)) {
            int cnt = 0;
            /* Neighbors from the row above */
            if (col > 0)        cnt += !!(above & (1u << (col - 1)));
            cnt += !!(above & (1u << col));
            if (col < 5)        cnt += !!(above & (1u << (col + 1)));
            /* Neighbors from the same row (excluding itself) */
            if (col > 0)        cnt += !!(row & (1u << (col - 1)));
            if (col < 5)        cnt += !!(row & (1u << (col + 1)));
            /* Neighbors from the row below */
            if (col > 0)        cnt += !!(below & (1u << (col - 1)));
            cnt += !!(below & (1u << col));
            if (col < 5)        cnt += !!(below & (1u << (col + 1)));
            if (cnt < 3 || cnt > 5) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, so n>=1 is expected */

    /* Constant limits */
    enum { MASKS = 64, PAIRS = 4096 };

    /* Static tables initialized once */
    static bool initialized = false;
    static unsigned char row_zero_valid[64];
    static unsigned char pair_init_valid[PAIRS];
    static unsigned char pair_last_valid[PAIRS];
    static unsigned char pair_trans_cnt[PAIRS];
    static unsigned char pair_trans[PAIRS][64];
    static seq_t dp[64][64];
    static seq_t dp_next[64][64];

    if (!initialized) {
        /* Pre‑compute validity of a row with both upper and lower rows zero. */
        for (int m = 0; m < 64; ++m) {
            row_zero_valid[m] = validate_row((uint8_t)m, 0, 0);
        }

        /* Pre‑compute validity of a pair (prev, curr) and transitions. */
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                int idx = prev * 64 + curr;

                /* Transition list: all nxt where curr is valid given prev and nxt. */
                unsigned char cnt = 0;
                for (int nxt = 0; nxt < 64; ++nxt) {
                    if (validate_row((uint8_t)curr, (uint8_t)prev, (uint8_t)nxt)) {
                        pair_trans[idx][cnt++] = (unsigned char)nxt;
                    }
                }
                pair_trans_cnt[idx] = cnt;

                /* Validity of the last row for this pair. */
                pair_last_valid[idx] = validate_row((uint8_t)curr, (uint8_t)prev, 0);

                /* Validity of the first row (prev) when above is zero. */
                pair_init_valid[idx] = validate_row((uint8_t)prev, 0, (uint8_t)curr);
            }
        }
        initialized = true;
    }

    /* Handle n==1 directly: single row with no neighbours. */
    if (n == 1) {
        seq_t ans = 0;
        for (int m = 0; m < 64; ++m)
            if (row_zero_valid[m]) ans += 1;
        return ans;
    }

    /* Reset DP arrays */
    mem_set(dp, 0, sizeof(dp));
    mem_set(dp_next, 0, sizeof(dp_next));

    /* Initial DP: first two rows. */
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            if (pair_init_valid[prev * 64 + curr])
                dp[prev][curr] = 1;
        }
    }

    /* If only two rows, finalize with the last row check. */
    if (n == 2) {
        seq_t ans = 0;
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t val = dp[prev][curr];
                if (val && pair_last_valid[prev * 64 + curr])
                    ans += val;
            }
        }
        return ans;
    }

    /* DP for rows 3..n */
    int remaining = (int)n - 2;
    for (int step = 0; step < remaining; ++step) {
        mem_set(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                int idx = prev * 64 + curr;
                unsigned char cnt = pair_trans_cnt[idx];
                unsigned char *list = pair_trans[idx];
                for (int k = 0; k < cnt; ++k) {
                    int nxt = list[k];
                    dp_next[curr][nxt] += val;
                }
            }
        }
        mem_copy(dp, dp_next, sizeof(dp));
    }

    /* Final answer: validate the last row. */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            seq_t val = dp[prev][curr];
            if (!val) continue;
            if (pair_last_valid[prev * 64 + curr])
                ans += val;
        }
    }
    return ans;
}
