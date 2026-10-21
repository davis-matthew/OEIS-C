/*
 * A297975  Number of nX5 0..1 arrays with every element equal to 2, 3, 4 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n × 5 binary matrices such that each entry
 * has between 2 and 5 equal neighboring cells (king moves), with the
 * upper‑left entry fixed to 0.
 * The routine builds all possible row patterns (2^5) and uses dynamic
 * programming to count how many n‑row configurations satisfy the
 * adjacency constraints.
 */

#include <stdbool.h>
#include <stddef.h> /* for size_t */

/* Compile‑time constants for the 5‑column grid */
enum { S = 32, SENTINEL = S }; /* S = 2^5, sentinel index = 32 */

/* Check if a single row `cur` is valid given the (possibly absent)
 * neighboring rows `above` and `below`. */
static bool row_ok(unsigned int cur,
                   bool has_above, unsigned int above,
                   bool has_below, unsigned int below)
{
    for (int c = 0; c < 5; ++c) {
        int bit = (int)((cur >> c) & 1U);
        int cnt = 0;

        /* same row: left and right */
        if (c > 0)   cnt += ((int)((cur >> (c - 1)) & 1U) == bit);
        if (c < 4)   cnt += ((int)((cur >> (c + 1)) & 1U) == bit);

        /* above row */
        if (has_above) {
            if (c > 0)   cnt += ((int)((above >> (c - 1)) & 1U) == bit);
            cnt += ((int)((above >> c) & 1U) == bit);
            if (c < 4)   cnt += ((int)((above >> (c + 1)) & 1U) == bit);
        }

        /* below row */
        if (has_below) {
            if (c > 0)   cnt += ((int)((below >> (c - 1)) & 1U) == bit);
            cnt += ((int)((below >> c) & 1U) == bit);
            if (c < 4)   cnt += ((int)((below >> (c + 1)) & 1U) == bit);
        }

        if (cnt < 2 || cnt > 5) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1 */

    /* Pre‑computed validity tables (built once) */
    static bool valid[SENTINEL + 1][S][S];
    static bool final_ok[SENTINEL + 1][S];
    static bool tables_built = false;

    if (!tables_built) {
        for (unsigned int p = 0; p <= SENTINEL; ++p) {
            bool has_above = (p != SENTINEL);
            unsigned int above = (has_above ? p : 0U);
            for (unsigned int u = 0; u < S; ++u) {
                for (unsigned int v = 0; v < S; ++v) {
                    /* row `u` is valid given above `p` and below `v` */
                    valid[p][u][v] =
                        row_ok(u, has_above, above, true, v);
                }
                /* final row: only above present, no below */
                final_ok[p][u] =
                    row_ok(u, has_above, above, false, 0U);
            }
        }
        tables_built = true;
    }

    /* DP tables: dp[prev][cur] holds counts for processed rows,
     * where `prev` is the pattern of the previous row
     * (SENTINEL for the row before the first). */
    seq_t dp[SENTINEL + 1][S];
    seq_t ndp[SENTINEL + 1][S];

    /* initialise all to zero */
    for (unsigned int i = 0; i <= SENTINEL; ++i)
        for (unsigned int j = 0; j < S; ++j)
            dp[i][j] = (seq_t)0;

    /* First row: upper-left element must be 0 */
    for (unsigned int pattern = 0; pattern < S; ++pattern) {
        if ((pattern & 1U) == 0)   /* leftmost column bit is 0 */
            dp[SENTINEL][pattern] = (seq_t)1;
    }

    /* Process subsequent rows */
    for (idx_t step = 2; step <= n; ++step) {
        /* reset ndp */
        for (unsigned int i = 0; i <= SENTINEL; ++i)
            for (unsigned int j = 0; j < S; ++j)
                ndp[i][j] = (seq_t)0;

        for (unsigned int prev = 0; prev <= SENTINEL; ++prev) {
            for (unsigned int cur = 0; cur < S; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == (seq_t)0) continue;
                for (unsigned int nxt = 0; nxt < S; ++nxt) {
                    if (valid[prev][cur][nxt]) {
                        ndp[cur][nxt] += ways;
                    }
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (unsigned int i = 0; i <= SENTINEL; ++i)
            for (unsigned int j = 0; j < S; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final validation for the last row (no below row) */
    seq_t total = (seq_t)0;
    for (unsigned int prev = 0; prev <= SENTINEL; ++prev) {
        for (unsigned int cur = 0; cur < S; ++cur) {
            seq_t ways = dp[prev][cur];
            if (ways == (seq_t)0) continue;
            if (final_ok[prev][cur])
                total += ways;
        }
    }

    return total;
}
