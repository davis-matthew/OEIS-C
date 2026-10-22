/*
 * A360215  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct nonzero icosahedral numbers in exactly n ways,
 *          or -1 if no such integer exists.
 * https://oeis.org/A360215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer that can be expressed as the
 *            sum of distinct nonzero icosahedral numbers in exactly n ways
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LIMIT_N 10000
#define MAX_SUM 2000000

static seq_t answers[LIMIT_N + 1];
static bool initialized = false;
static uint16_t dp[MAX_SUM + 1];
static uint32_t coins[256];
static size_t coin_count = 0;

static void init(void)
{
    if (initialized)
        return;

    /* initialise result array with sentinel (-1) */
    for (size_t i = 0; i <= LIMIT_N; ++i)
        answers[i] = (seq_t)-1;

    /* generate icosahedral numbers <= MAX_SUM */
    coin_count = 0;
    for (uint64_t k = 1;; ++k) {
        uint64_t ico = k * (5 * k * k - 5 * k + 2) / 2; /* formula */
        if (ico > (uint64_t)MAX_SUM)
            break;
        coins[coin_count++] = (uint32_t)ico;
    }

    /* initialise dp array */
    memset(dp, 0, sizeof(dp));
    dp[0] = 1; /* empty subset */

    /* subset-sum DP: count subsets for each sum, capped at LIMIT_N+1 */
    for (size_t ci = 0; ci < coin_count; ++ci) {
        uint32_t val = coins[ci];
        for (int sum = MAX_SUM; sum >= (int)val; --sum) {
            uint16_t add = dp[sum - val];
            if (!add)
                continue;
            uint16_t new_val = dp[sum] + add;
            if (new_val > LIMIT_N)
                new_val = LIMIT_N + 1; /* cap to avoid overflow */
            dp[sum] = new_val;
        }
    }

    /* find minimal sum for each representation count */
    for (size_t sum = 1; sum <= MAX_SUM; ++sum) {
        uint16_t cnt = dp[sum];
        if (cnt >= 1 && cnt <= LIMIT_N && answers[cnt] == (seq_t)-1)
            answers[cnt] = (seq_t)sum;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > LIMIT_N)
        return (seq_t)-1;
    init();
    return answers[(size_t)n];
}
