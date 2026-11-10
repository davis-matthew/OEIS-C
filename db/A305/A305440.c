/*
 * A305440  Indices k such that A292518(k) = 0.
 * https://oeis.org/A305440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with [x^k] Product_{j>=1}(1 - x^(j(j+1)/2)) =
 *            0; computed by a DP sieve over triangular numbers (search limit
 *            20000, conjectured finite, last term 3957)
 * Range:     u8 n <= 44, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 72 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A305440_LIMIT 20000

static int A305440_cnt = -1;
static int A305440_idx[A305440_LIMIT + 1];

static void A305440_init(void)
{
    int *dp = (int *)calloc(A305440_LIMIT + 1, sizeof(int));
    int N = A305440_LIMIT;
    A305440_cnt = 0;
    if (!dp)
        return;
    dp[0] = 1;
    for (int k = 1;; ++k) {
        int t = k * (k + 1) / 2;
        if (t > N)
            break;
        for (int j = N; j >= t; --j)
            dp[j] -= dp[j - t];
    }
    for (int k = 1; k <= N; ++k)
        if (dp[k] == 0)
            A305440_idx[A305440_cnt++] = k;
    free(dp);
}

seq_t a(idx_t n)
{
    if (A305440_cnt < 0)
        A305440_init();
    if (n < 1 || n > (idx_t)A305440_cnt)
        return 0;
    return (seq_t)A305440_idx[n - 1];
}
