/*
 * A383072  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 4.
 * https://oeis.org/A383072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of n-digit positive integers where every pair of
 *            consecutive * digits differ by at least 4. Computed by dynamic
 *            programming
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Below offset (n==0) is irrelevant; return 0. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 9; /* digits 1-9 */

    seq_t prev[10] = {0};
    seq_t curr[10];

    /* Length 1 numbers: first digit 1-9 */
    for (int d = 1; d <= 9; ++d)
        prev[d] = 1;

    for (idx_t pos = 2; pos <= n; ++pos) {
        /* Reset current counts */
        for (int k = 0; k < 10; ++k)
            curr[k] = 0;

        /* Transition from previous digit d to next digit k */
        for (int d = 0; d < 10; ++d) {
            seq_t cnt = prev[d];
            if (!cnt)
                continue;
            for (int k = 0; k < 10; ++k) {
                int diff = k - d;
                if (diff < 0)
                    diff = -diff;
                if (diff >= 4)
                    curr[k] += cnt;
            }
        }

        /* Prepare for next iteration */
        for (int i = 0; i < 10; ++i)
            prev[i] = curr[i];
    }

    /* Sum over all possible last digits */
    seq_t total = 0;
    for (int i = 0; i < 10; ++i)
        total += prev[i];
    return total;
}
