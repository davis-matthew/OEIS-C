/*
 * A289354  Number of 9-digit numbers whose sum of digits is n.
 * https://oeis.org/A289354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, changed
 * Formula:   a(n) = number of 9‑digit numbers whose digits sum to n, computed
 *            by dynamic programming over the digits
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    const idx_t MAX_SUM = 81;
    static bool initialized = false;
    static seq_t dp[82];      /* dp[s] = number of 9‑digit numbers with digit sum s */

    if (!initialized) {
        /* first (most significant) digit: 1..9 */
        for (idx_t d = 1; d <= 9; ++d) {
            dp[d] += 1;
        }

        /* add the remaining 8 digits, each 0..9 */
        for (idx_t pos = 0; pos < 8; ++pos) {
            seq_t newdp[82] = {0};

            for (idx_t s = 0; s <= MAX_SUM; ++s) {
                seq_t ways = dp[s];
                if (!ways) continue;
                for (idx_t d = 0; d <= 9; ++d) {
                    idx_t ns = s + d;
                    if (ns > MAX_SUM) break;
                    newdp[ns] += ways;
                }
            }

            for (idx_t i = 0; i <= MAX_SUM; ++i) {
                dp[i] = newdp[i];
            }
        }

        initialized = true;
    }

    if (n < 1 || n > MAX_SUM)
        return 0;
    return dp[(size_t)n];
}
