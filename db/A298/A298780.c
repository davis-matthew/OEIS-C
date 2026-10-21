/*
 * A298780  Number of nX6 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with upper‑left cell 0 where
 *            each cell has 0, 3, 4, 5 or 8 king‑move neighbors of the same
 *            value. (OEIS A298780)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   is_valid_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_valid_row.h"

static uint8_t topNext[64][64],   /* next rows valid for a top row cur */
               middleNext[64][64][64]; /* next rows valid for a middle row cur given prev */
static uint8_t topCnt[64], midCnt[64][64];
static bool bottomOk[64][64];
static bool initialized = false;

/* Pre‑compute allowed transitions and bottom check once.  */
static void init(void)
{
    if (initialized) return;

    /* Top row transitions (no previous row). */
    for (int cur = 0; cur < 64; ++cur) {
        topCnt[cur] = 0;
        for (int next = 0; next < 64; ++next) {
            if (is_valid_row((uint8_t)cur, (uint8_t)0,
                             (uint8_t)next, false, true))
                topNext[cur][topCnt[cur]++] = (uint8_t)next;
        }
    }

    /* Middle row transitions. */
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            midCnt[prev][cur] = 0;
            for (int next = 0; next < 64; ++next) {
                if (is_valid_row((uint8_t)cur, (uint8_t)prev,
                                 (uint8_t)next, true, true))
                    middleNext[prev][cur][midCnt[prev][cur]++] =
                        (uint8_t)next;
            }
        }
    }

    /* Bottom row test (no next row). */
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            bottomOk[prev][cur] =
                is_valid_row((uint8_t)cur, (uint8_t)prev,
                             (uint8_t)0, true, false);
        }
    }

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    if (n == 1) return (seq_t)1;   /* only pattern 010101 with upper‑left 0 */

    init();

    /* Two DP arrays to swap each iteration. */
    static seq_t dp1[64][64], dp2[64][64];
    seq_t (*curr)[64] = dp1;
    seq_t (*next)[64] = dp2;

    memset(curr, 0, sizeof dp1);

    /* Initial two rows.  prev = row0, cur = row1. */
    for (int r0 = 0; r0 < 64; ++r0) {
        if (r0 & 1) continue;            /* upper‑left must be 0 */
        for (int k = 0; k < topCnt[r0]; ++k) {
            uint8_t r1 = topNext[r0][k];
            curr[r0][r1] += (seq_t)1;
        }
    }

    if (n == 2) {
        seq_t ans = (seq_t)0;
        for (int prev = 0; prev < 64; ++prev)
            for (int cur = 0; cur < 64; ++cur)
                if (curr[prev][cur] && bottomOk[prev][cur])
                    ans += curr[prev][cur];
        return ans;
    }

    /* Process rows 3 … n.  Each step adds one more row. */
    for (idx_t step = 2; step <= n - 1; ++step) {
        memset(next, 0, sizeof dp2);
        for (int prev = 0; prev < 64; ++prev)
            for (int cur = 0; cur < 64; ++cur) {
                seq_t val = curr[prev][cur];
                if (val == 0) continue;
                uint8_t cnt = midCnt[prev][cur];
                for (int k = 0; k < cnt; ++k) {
                    uint8_t nxt = middleNext[prev][cur][k];
                    next[cur][nxt] += val;
                }
            }
        /* swap arrays */
        seq_t (*tmp)[64] = curr;
        curr = next;
        next = tmp;
    }

    /* Final row must satisfy bottom constraint. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 64; ++prev)
        for (int cur = 0; cur < 64; ++cur)
            if (curr[prev][cur] && bottomOk[prev][cur])
                ans += curr[prev][cur];

    return ans;
}
