/*
 * A341001  Number of partitions of n into 10 distinct nonzero squares.
 * https://oeis.org/A341001
 *
 * Offset:    385    (a(385) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 10 distinct nonzero
 *            squares
 * Range:     u8 n <= 1080, u16 n <= 3084, u32 n <= 9183, u64 n <= 9183,
 *            u128 n <= 9183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_N 10000
#define K 10

static seq_t dp[MAX_N + 1][K + 1] = {0};

static int initialized = 0;

static void init(void)
{
    /* dp[s][c] = number of ways to form sum s using exactly c distinct squares
       processed so far. */
    dp[0][0] = 1;
    for (idx_t k = 1; k * k <= MAX_N; ++k)
    {
        idx_t sq = k * k;
        for (idx_t s = MAX_N; s >= sq; --s)
        {
            for (idx_t c = 1; c <= K; ++c)
            {
                dp[s][c] += dp[s - sq][c - 1];
            }
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init();
    if (n > MAX_N)
        return 0;
    return dp[n][K];
}
