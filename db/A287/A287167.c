/*
 * A287167  Smallest number with exactly n representations as a sum of 8
 *          nonzero squares or 0 if no such number exists.
 * https://oeis.org/A287167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this implementation generates a(n) by computing the number of
 *            unordered representations of every integer as a sum of exactly 8
 *            non‑zero squares, then memorizing, for each count n, the
 *            smallest integer that yields that count
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) returns seq_t* */

/*  The maximum count of representations that will be recorded.
    Adjust if you need to answer queries for larger n. */
#define MAX_COUNT 100000

/*  Upper bound of numbers whose representations will be examined.
    The limit should be large enough that the minimal number for any
    requested count (up to MAX_COUNT) lies within it. */
#define LIMIT 210000

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *min_cnt = NULL;
    static size_t max_known = 0;
    static uint32_t *dp = NULL;   /* dp[9][LIMIT+1] flattened */

    if (!initialized) {
        /* ---------- 1. Allocate and initialise DP table  ---------- */
        /* dp has 9 rows (0..8) and (LIMIT+1) columns.
           We store it as a single linear array of uint32_t. */
        size_t rows = LIMIT + 1;
        size_t cols = 9;
        dp = (uint32_t *)calloc(rows * cols, sizeof(uint32_t));
        if (!dp) return 0;   /* allocation failure */

        /* base case: one way to sum to 0 using 0 squares */
        dp[0 * cols + 0] = 1u;

        /* ---------- 2. Populate DP for all squares ≤ LIMIT ---------- */
        for (uint32_t s = 1; ; ++s) {
            uint32_t sq = s * s;
            if (sq > LIMIT) break;
            /* update counts for using at least one square and
               at most 8 squares (t = 1..8) */
            for (int t = 1; t <= 8; ++t) {
                for (uint32_t sum = sq; sum <= LIMIT; ++sum) {
                    uint32_t idx   = sum * cols + t;
                    uint32_t prev  = (sum - sq) * cols + (t - 1);
                    uint32_t val   = dp[idx] + dp[prev];
                    if (val > MAX_COUNT) val = MAX_COUNT;  /* cap to avoid overflow */
                    dp[idx] = val;
                }
            }
        }

        /* ---------- 3. Allocate result array and fill minimal N's ---------- */
        min_cnt = alloc_seq_array(MAX_COUNT + 1);
        if (!min_cnt) { free(dp); return 0; }

        /* Scan all sums in increasing order.
           The first time we see a count c (≤ MAX_COUNT) we record
           the current sum as the smallest integer with that count. */
        for (size_t sum = 0; sum <= LIMIT; ++sum) {
            uint32_t c = dp[sum * cols + 8];
            if (c == 0 || c > MAX_COUNT) continue;
            if (min_cnt[c] == 0) {
                min_cnt[c] = (seq_t)sum;
                if (c > max_known) max_known = c;
            }
        }

        /* Clean up DP table; results are now in min_cnt. */
        free(dp);
        dp = NULL;
        initialized = true;
    }

    /* ---------- 4. Return the answer for the requested n ---------- */
    if (n == 0) return 0;            /* a(0) is undefined by offset */
    if (n > max_known) return 0;     /* no such number within computed range */
    return min_cnt[n];
}
