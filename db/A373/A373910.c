/*
 * A373910  Number of compositions of 7*n into parts 4 and 7.
 * https://oeis.org/A373910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of 7*n into parts 4 and 7
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is equivalent to the recurrence */
/*   dp[s] = dp[s-4] + dp[s-7]  (dp[0] = 1)   for s = 7*n */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* External types seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* Static cache of compositions for sums 0..computed_sum. */
    static seq_t *dp = NULL;      /* array of seq_t values */
    static size_t dp_len = 0;     /* current allocated length (dp_len > 0) */
    static idx_t computed_sum = 0; /* largest sum that has a valid entry */

    /* Sum to be queried: s = 7*n. */
    idx_t sum = 7 * n;
    size_t sum_s = (size_t)sum;  /* array index, safe for n up to ~10⁵ */

    /* Initial allocation on first call. */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!dp) return (seq_t)0;   /* fall back on overflow, though this should not occur */
        dp[0] = (seq_t)1;           /* only one way to compose 0 */
        dp_len = 1;
        computed_sum = 0;
    }

    /* Grow the cache if we haven't computed this sum yet. */
    if (sum_s >= dp_len) {
        size_t new_len = sum_s + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure, return 0 as safe fallback */
        dp = tmp;
        dp_len = new_len;

        /* Compute new dp values up to sum_s. */
        for (size_t i = (size_t)computed_sum + 1; i <= sum_s; ++i) {
            seq_t v = (seq_t)0;
            if (i >= 4) v += dp[i - 4];
            if (i >= 7) v += dp[i - 7];
            dp[i] = v;
        }
        computed_sum = sum_s;
    }

    return dp[sum_s];
}
