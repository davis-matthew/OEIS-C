/*
 * A383200  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 5.
 * https://oeis.org/A383200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of positive integers with n digits in which
 *            adjacent digits differ by at most 5. Computed by dynamic
 *            programming: let dp_k[d] be the number of length‑k strings
 *            ending with digit d. Recurrence: dp_1[d] = 1 for 1 ≤ d ≤ 9,
 *            dp_1[0] = 0 dp_{k+1}[d] = Σ_{|p - d| ≤ 5} dp_k[p] (0 ≤ d,p ≤
 *            9)...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset starts at 1 */

    seq_t dp[10] = {0};
    for (int d = 1; d <= 9; ++d)
        dp[d] = 1;              /* length 1, leading digit non‑zero */

    if (n == 1) {
        seq_t sum = 0;
        for (int d = 1; d <= 9; ++d)
            sum += dp[d];
        return sum;
    }

    seq_t dp_next[10];
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d <= 9; ++d) {
            seq_t s = 0;
            int low  = d - 5;
            if (low < 0) low = 0;
            int high = d + 5;
            if (high > 9) high = 9;
            for (int p = low; p <= high; ++p)
                s += dp[p];
            dp_next[d] = s;
        }
        /* copy dp_next to dp for next iteration */
        for (int d = 0; d <= 9; ++d)
            dp[d] = dp_next[d];
    }

    seq_t result = 0;
    for (int d = 0; d <= 9; ++d)
        result += dp[d];
    return result;
}
