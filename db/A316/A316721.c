/*
 * A316721  Expansion of Product_{k=1..9} (1+x^(2*k-1))/(1-x^(2*k)).
 * https://oeis.org/A316721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..9} (1 + x^(2k-1))/(1 -
 *            x^(2k))
 * Range:     u8 n <= 22, u16 n <= 63, u32 n <= 292, u64 n <= 4769,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_DP_SIZE 20000   /* supports n up to at least 10000 */

/* DP table: a[i] is the sequence value for index i */
static seq_t dp[MAX_DP_SIZE];
static idx_t dp_max = (idx_t)-1;   /* -1 indicates no values computed yet */

/* Compute the sequence up to (and including) max_n.
   The computation does not rely on previous results. */
static void recompute(idx_t max_n)
{
    if (max_n >= MAX_DP_SIZE)        /* safety: should not be reached for valid n */
        return;

    /* Initialize DP array */
    for (idx_t i = 0; i <= max_n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Bounded odd parts: each used at most once.
       Process in reverse order to prevent reuse in a single step. */
    const idx_t odd_parts[9] = {1, 3, 5, 7, 9, 11, 13, 15, 17};
    for (int p = 0; p < 9; ++p) {
        idx_t o = odd_parts[p];
        for (idx_t i = max_n; i >= o; --i) {
            dp[i] = dp[i] + dp[i - o];
        }
    }

    /* Unlimited even parts: standard coin‑change forward loops. */
    const idx_t even_parts[9] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
    for (int p = 0; p < 9; ++p) {
        idx_t e = even_parts[p];
        for (idx_t i = e; i <= max_n; ++i) {
            dp[i] = dp[i] + dp[i - e];
        }
    }

    dp_max = max_n;
}

seq_t a(idx_t n)
{
    if (dp_max == (idx_t)-1 || n > dp_max)
        recompute(n);
    return dp[n];
}
