/*
 * A241951  Number of possible representations of n as a sum of distinct
 *          positive integers from the Fibonacci and Lucas sequences
 *          0,1,1,2,3,5,8,13,... and 2,1,3,4,7,11,... (A000045 and A000032).
 * https://oeis.org/A241951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 1978, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   coin_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ways to write n as a sum of distinct
 * Fibonacci and Lucas numbers (both sequences are included).
 * This is a standard subset‑sum counting problem.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/coin_exists.h"

/* Main function required by the project */
seq_t a(idx_t n) {
    /* Handle the trivial case */
    if (n == 0) return (seq_t)1;

    /* Coins (distinct Fibonacci or Lucas numbers) */
    unsigned long long coins[64];
    size_t cnt = 0;
    unsigned long long max = n;

    /* Fibonacci numbers: 1,1,2,3,...  (start with F1=1, F2=1) */
    unsigned long long f1 = 1, f2 = 1;
    while (f1 <= max) {
        if (!coin_exists(coins, cnt, f1)) {
            coins[cnt++] = f1;
        }
        unsigned long long next = f1 + f2;
        f1 = f2;
        f2 = next;
    }

    /* Lucas numbers: 2,1,3,4,7,11,... (start with L0=2, L1=1) */
    unsigned long long l0 = 2, l1 = 1;
    while (l0 <= max) {
        if (!coin_exists(coins, cnt, l0)) {
            coins[cnt++] = l0;
        }
        unsigned long long next = l0 + l1;
        l0 = l1;
        l1 = next;
    }

    /* Dynamic programming array */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) abort();

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* For each coin, update dp in reverse to avoid reuse */
    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long coin = coins[i];
        for (idx_t s = n; s >= coin; --s) {
            dp[s] = dp[s] + dp[s - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
