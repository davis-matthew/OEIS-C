/*
 * A383199  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 4.
 * https://oeis.org/A383199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=9; for n>1,
   a(n) = Σ_{d=0}^9 f(n,d) where
     f(1,d) = 1 for d≠0, 0 for d=0
     f(k,d) = Σ_{|p−d|≤4} f(k−1,p)   for k>1
   (dp[n][d] counts n‑digit numbers ending in digit d). */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t dp_prev[10];
    for (int d = 0; d < 10; ++d)
        dp_prev[d] = (d == 0) ? 0 : 1;

    if (n == 1) {
        seq_t total = 0;
        for (int d = 0; d < 10; ++d)
            total += dp_prev[d];
        return total;
    }

    seq_t dp_cur[10];
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 10; ++d) {
            seq_t sum = 0;
            int low  = d - 4; if (low < 0)  low = 0;
            int high = d + 4; if (high > 9) high = 9;
            for (int p = low; p <= high; ++p)
                sum += dp_prev[p];
            dp_cur[d] = sum;
        }
        for (int d = 0; d < 10; ++d)
            dp_prev[d] = dp_cur[d];
    }

    seq_t total = 0;
    for (int d = 0; d < 10; ++d)
        total += dp_prev[d];
    return total;
}
