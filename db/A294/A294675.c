/*
 * A294675  Numbers that are the sum of 5 nonzero squares in exactly 1 way.
 * https://oeis.org/A294675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer that can be expressed as the sum of five
   nonzero squares in exactly one way (unordered). */

#include <stdlib.h>
#include <stdint.h>

/* Use a wider type for internal counts to avoid overflow of seq_t. */
typedef uint32_t cnt_type;

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static int initialized = 0;
    static seq_t *terms = NULL;
    static idx_t term_count = 0;
    static cnt_type *dp[6] = { NULL };

    if (!initialized) {
        /* ----- 1. Prepare DP table ----------------------------------- */
        const idx_t LIMIT = 50000;          /* proven upper bound for terms */

        /* Allocate DP arrays */
        for (int i = 0; i <= 5; ++i)
            dp[i] = (cnt_type *)calloc((size_t)(LIMIT + 1), sizeof(cnt_type));

        /* Base case: one way to make 0 with 0 squares */
        dp[0][0] = 1;

        /* Find the largest square ≤ LIMIT */
        idx_t max_w = 0;
        while ((max_w + 1) * (max_w + 1) <= LIMIT)
            ++max_w;

        /* DP: add squares one by one */
        for (idx_t w = 1; w <= max_w; ++w) {
            idx_t sq = w * w;
            for (int p = 1; p <= 5; ++p) {
                for (idx_t s = sq; s <= LIMIT; ++s)
                    dp[p][s] += dp[p - 1][s - sq];
            }
        }

        /* ----- 2. Extract the numbers that occur exactly once ----- */
        idx_t cnt = 0;
        for (idx_t num = 1; num <= LIMIT; ++num)
            if (dp[5][num] == 1)
                ++cnt;

        terms = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t num = 1; num <= LIMIT; ++num)
            if (dp[5][num] == 1)
                terms[pos++] = (seq_t)num;

        term_count = cnt;
        initialized = 1;
    }

    /* ----- 3. Answer the query ------------------------------------- */
    if (n == 0 || n > term_count)
        return (seq_t)0;          /* undefined for out‑of‑range indices */

    return terms[n - 1];
}
