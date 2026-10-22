/*
 * A383197  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 2.
 * https://oeis.org/A383197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of positive n‑digit integers in base 10 where each
 *            pair of adjacent digits differ by at most 2. Recurrence: dp[d] =
 *            number of length‑len sequences ending in digit d. For length 1:
 *            dp[d] = 1 (d = 1..9), dp[0] = 0. For each subsequent length:
 *            dp_next[d] = Σ_{k=max(0,d-2)}^{min(9,d+2)}...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    seq_t dp[10], dp_next[10];

    /* initial length 1 */
    for (int d = 0; d < 10; ++d)
        dp[d] = (d == 0) ? (seq_t)0 : (seq_t)1;

    if (n == 1) {
        seq_t res = 0;
        for (int d = 0; d < 10; ++d) res += dp[d];
        return res;
    }

    /* iterate lengths 2..n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 10; ++d) {
            seq_t sum = 0;
            int lo = d - 2; if (lo < 0) lo = 0;
            int hi = d + 2; if (hi > 9) hi = 9;
            for (int k = lo; k <= hi; ++k)
                sum += dp[k];
            dp_next[d] = sum;
        }
        /* copy next to current */
        for (int d = 0; d < 10; ++d)
            dp[d] = dp_next[d];
    }

    seq_t result = 0;
    for (int d = 0; d < 10; ++d)
        result += dp[d];
    return result;
}
