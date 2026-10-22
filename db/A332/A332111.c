/*
 * A332111  Numbers that are not the sum of eleven (11) positive cubes.
 * https://oeis.org/A332111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th integer that cannot be expressed as the sum of *
 *            eleven positive cubes. The sequence is finite; the last term is
 *            a(92)=321. * This implementation builds the list on first call
 *            using a dynamic * programming sieve and serves subsequent
 *            queries from that list
 * Range:     u8 n <= 91, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The sequence contains only 92 terms; we keep a buffer large enough. */
    static seq_t list[100];
    static size_t list_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Upper bound for all needed terms (the last known term is 321). */
        const int MAX = 321;

        /* DP table: dp[steps][sum] indicates whether 'sum' can be obtained
         * as the sum of 'steps' positive cubes. */
        bool dp[12][MAX + 1];
        memset(dp, 0, sizeof(dp));
        dp[0][0] = true;   /* 0 sum using 0 cubes is possible */

        /* Pre‑compute all positive cubes not exceeding MAX. */
        int cubes[10];   /* cube(root(321)) < 10, safe upper bound */
        int cube_cnt = 0;
        for (int c = 1; ; ++c) {
            int cube = c * c * c;
            if (cube > MAX) break;
            cubes[cube_cnt++] = cube;
        }

        /* Build DP for 1 … 11 cubes. */
        for (int step = 1; step <= 11; ++step) {
            for (int sum = 0; sum <= MAX; ++sum) {
                if (!dp[step - 1][sum]) continue;
                for (int i = 0; i < cube_cnt; ++i) {
                    int v = sum + cubes[i];
                    if (v > MAX) break;
                    dp[step][v] = true;
                }
            }
        }

        /* Collect all integers that are NOT representable with 11 cubes. */
        for (int m = 1; m <= MAX; ++m) {
            if (!dp[11][m]) {
                list[list_len++] = (seq_t)m;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > list_len) return 0;
    return list[n - 1];
}
