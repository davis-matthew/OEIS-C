/*
 * A295702  Largest number with exactly n representations as a sum of six
 *          positive squares.
 * https://oeis.org/A295702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A295702: largest number with exactly n representations as a
   sum of six positive squares, using dynamic programming over unordered
   score combinations. */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Auxiliary constants -----------------------------------------------------*/
#define MAX_N 10000            /* maximum index we support (adjust as needed) */
#define MAX_M 50000            /* search bound for the sum (adjust if needed) */

/* ------------------------------------------------------------------------*/
static seq_t *best_arr = NULL;  /* best[n] stores largest m with count n */
static bool initialized = false;

/* ------------------------------------------------------------------------*/
/* Helper: zero‑initialize the DP table and build the counts of unordered
   6‑square representations up to MAX_M.  The DP table stores counts
   capped at MAX_N + 1. */
static void precompute(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Allocate DP table: dp[m][k] = number of ways to write m as sum of k
       unordered squares.  Use a flat array to avoid 2‑D indirection. */
    const size_t dp_size = (size_t)(MAX_M + 1) * 7;      /* 0..MAX_M, k=0..6   */
    uint16_t *dp = (uint16_t *)calloc(dp_size, sizeof(uint16_t));
    if (!dp)
        exit(EXIT_FAILURE);

    /* Allocate answer array */
    best_arr = (seq_t *)calloc((size_t)MAX_N + 1, sizeof(seq_t));
    if (!best_arr) {
        free(dp);
        exit(EXIT_FAILURE);
    }

    /* Base case: zero sum with zero squares */
    dp[0] = 1;   /* dp[0][0] */

    /* Process each square value in ascending order.  This ensures each
       unordered multiset is counted exactly once. */
    const int max_sq_root = (int)floor(sqrt((double)MAX_M));
    for (int a = 1; a <= max_sq_root; ++a) {
        const int sq = a * a;
        for (int sum = sq; sum <= MAX_M; ++sum) {
            const size_t idx_base = (size_t)sum * 7;
            const size_t idx_prev_base = (size_t)(sum - sq) * 7;
            for (int k = 1; k <= 6; ++k) {
                uint16_t prev_cnt = dp[idx_prev_base + (k - 1)];
                if (!prev_cnt)
                    continue;
                uint16_t cur_cnt = dp[idx_base + k];
                /* Add and cap at MAX_N + 1 to avoid overflow. */
                uint32_t new_cnt = (uint32_t)cur_cnt + (uint32_t)prev_cnt;
                if (new_cnt > (uint32_t)MAX_N + 1)
                    new_cnt = (uint32_t)MAX_N + 1;
                dp[idx_base + k] = (uint16_t)new_cnt;
            }
        }
    }

    /* Extract the best values: for each m, if it has a non‑zero count of
       exactly k = 6 squares, record the largest m for that count. */
    for (int m = 1; m <= MAX_M; ++m) {
        uint16_t cnt = dp[(size_t)m * 7 + 6];
        if (cnt && cnt <= MAX_N) {
            seq_t val = (seq_t)m;
            if (val > best_arr[cnt])
                best_arr[cnt] = val;
        }
    }

    free(dp);
}

/* ------------------------------------------------------------------------*/
/* Sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1 */
    if (n > MAX_N)
        return 0;          /* out of supported range */
    precompute();
    return best_arr[n];
}
