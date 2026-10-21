/*
 * A281834  Number of n X 5 0..1 arrays with no element equal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming over 5‑wide binary rows. * First element is
 *            forced to be 0. For every cell the number of * surrounding
 *            (king‑move) neighbors with the same value must be ≤ 4. * a(n)
 *            counts all n×5 arrays satisfying those constraints
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>


static bool is_row_valid(int prev, int curr, int next)
{
    for (int j = 0; j < 5; ++j) {
        int val = (curr >> j) & 1;
        int same = 0;
        /* same‑row neighbours */
        if (j > 0   && ((curr >> (j - 1)) & 1) == val) ++same;
        if (j < 4   && ((curr >> (j + 1)) & 1) == val) ++same;
        /* upper neighbours */
        if (prev != -1) {
            if (j > 0   && ((prev >> (j - 1)) & 1) == val) ++same;
            if (((prev >> j) & 1) == val) ++same;
            if (j < 4   && ((prev >> (j + 1)) & 1) == val) ++same;
        }
        /* lower neighbours */
        if (next != -1) {
            if (j > 0   && ((next >> (j - 1)) & 1) == val) ++same;
            if (((next >> j) & 1) == val) ++same;
            if (j < 4   && ((next >> (j + 1)) & 1) == val) ++same;
        }
        if (same > 4)
            return false;
    }
    return true;
}

static uint32_t next_valid_tbl[32][32];
static bool next_valid_ready = false;

static void precompute_next_valid(void)
{
    if (next_valid_ready) return;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                if (is_row_valid(a, b, c))
                    mask |= (1u << c);
            }
            next_valid_tbl[a][b] = mask;
        }
    }
    next_valid_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */
    if (n == 1) return 16;         /* top‑left must be 0, rest arbitrary */

    precompute_next_valid();

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];
    memset(dp, 0, sizeof(dp));

    /* initialise with rows 0 and 1 */
    for (int r0 = 0; r0 < 32; ++r0) {
        if (r0 & 1) continue;  /* top‑left (column 0 of row 0) must be 0 */
        for (int r1 = 0; r1 < 32; ++r1) {
            if (is_row_valid(-1, r0, r1))
                dp[r0][r1] += (seq_t)1;
        }
    }

    if (n == 2) {
        seq_t result = 0;
        for (int r0 = 0; r0 < 32; ++r0) {
            for (int r1 = 0; r1 < 32; ++r1) {
                seq_t ways = dp[r0][r1];
                if (!ways) continue;
                if (is_row_valid(r0, r1, -1))
                    result += ways;
            }
        }
        return result;
    }

    /* n ≥ 3: iterate rows 2 … n‑1 */
    for (idx_t row = 2; row < n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prevPrev = 0; prevPrev < 32; ++prevPrev) {
            for (int prev = 0; prev < 32; ++prev) {
                seq_t ways = dp[prevPrev][prev];
                if (!ways) continue;
                uint32_t mask = next_valid_tbl[prevPrev][prev];
                for (int next = 0; next < 32; ++next) {
                    if (mask & (1u << next))
                        dp_next[prev][next] += ways;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* final row validity */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways) continue;
            if (is_row_valid(prev, curr, -1))
                result += ways;
        }
    }
    return result;
}
