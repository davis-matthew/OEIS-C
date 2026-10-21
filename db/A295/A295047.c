/*
 * A295047  Number of n X 4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1's.
 * https://oeis.org/A295047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0/1 arrays in which every * 1 has either 0
 *            or 2 neighbors that are 1, counting horizontally * or
 *            vertically. The implementation uses a transfer‑matrix *
 *            dynamic‑programming algorithm that runs in O(n·256·16) time
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define STATE_BITS 4
#define STATE_COUNT (1u << STATE_BITS)

/* Precompute whether a triple of rows (prev, curr, next) satisfies the
 * adjacency rule for the middle row.  The table is indexed by the row
 * values (0..15). */
static bool valid[STATE_COUNT][STATE_COUNT][STATE_COUNT];
static bool valid_inited = false;

static void init_valid(void)
{
    if (valid_inited)
        return;
    valid_inited = true;

    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        for (int curr = 0; curr < STATE_COUNT; ++curr) {
            for (int next = 0; next < STATE_COUNT; ++next) {
                bool ok = true;
                for (int j = 0; j < STATE_BITS; ++j) {
                    int mask = 1 << j;
                    if (!(curr & mask))
                        continue;              /* only check 1's */
                    int deg = 0;
                    /* left neighbour */
                    if (j > 0 && (curr & (1 << (j - 1))))
                        ++deg;
                    /* right neighbour */
                    if (j < STATE_BITS - 1 && (curr & (1 << (j + 1))))
                        ++deg;
                    /* up neighbour */
                    if (prev & mask)
                        ++deg;
                    /* down neighbour */
                    if (next & mask)
                        ++deg;
                    if (deg != 0 && deg != 2) {
                        ok = false;
                        break;
                    }
                }
                valid[prev][curr][next] = ok;
            }
        }
    }
}

/* a(n): number of n×4 binary tables satisfying the adjacency
 * property.  n >= 1.  The function works for any seq_t and idx_t
 * types supplied by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined by offset, but safe to return 0 */

    init_valid();

    /* dp[prev*STATE_COUNT + curr] holds the number of ways to build
     * rows up to and including the current row (curr), with the
     * preceding row being prev, and all rows up to prev already valid. */
    seq_t dp[STATE_COUNT * STATE_COUNT] = {0};
    seq_t dp_next[STATE_COUNT * STATE_COUNT];

    /* first row: previous row is all zeros (dummy). */
    for (int curr = 0; curr < STATE_COUNT; ++curr)
        dp[curr] = 1;

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < sizeof(dp_next) / sizeof(dp_next[0]); ++i)
            dp_next[i] = 0;

        for (int prev = 0; prev < STATE_COUNT; ++prev) {
            for (int curr = 0; curr < STATE_COUNT; ++curr) {
                seq_t ways = dp[prev * STATE_COUNT + curr];
                if (ways == 0)
                    continue;
                for (int next = 0; next < STATE_COUNT; ++next) {
                    if (valid[prev][curr][next]) {
                        dp_next[curr * STATE_COUNT + next] += ways;
                    }
                }
            }
        }
        /* copy dp_next to dp for next iteration */
        for (size_t i = 0; i < sizeof(dp) / sizeof(dp[0]); ++i)
            dp[i] = dp_next[i];
    }

    /* final row must be validated against a dummy bottom row of zeros */
    seq_t ans = 0;
    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        for (int curr = 0; curr < STATE_COUNT; ++curr) {
            if (valid[prev][curr][0]) {
                ans += dp[prev * STATE_COUNT + curr];
            }
        }
    }
    return ans;
}
