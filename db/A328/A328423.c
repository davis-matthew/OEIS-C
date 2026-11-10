/*
 * A328423  Numbers in A328422.
 * https://oeis.org/A328423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328422(2*n), where A328422(m) is the number of paths
 *            from 2 to m via maps x -> x + x^j (j >= 0); computed by a DP
 *            over targets up to 2n
 * Range:     u8 n <= 19, u16 n <= 84, u32 n <= 590, u64 n <= 8143,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9896 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t N = 2 * n;
    seq_t *dp = mem_new(seq_t, (size_t)(N + 1));
    if (!dp)
        return 0;

    dp[2] = 1;

    for (idx_t y = 2; y <= N; ++y)
    {
        seq_t cur = dp[y];
        if (cur == 0)
            continue;

        /* j = 0: x -> x + 1 */
        if (y + 1 <= N)
            dp[y + 1] += cur;

        /* j >= 1: x -> x + x^j */
        idx_t power = y;
        while (power <= N - y)
        {
            dp[y + power] += cur;
            if (power > N / y)
                break;
            power *= y;
        }
    }

    seq_t result = dp[N];
    mem_free(dp);
    return result;
}
