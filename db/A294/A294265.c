/*
 * A294265  Number of partitions of n into squares that do not divide n.
 * https://oeis.org/A294265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into squares that do NOT divide n. For
 *            n = 0 the empty partition counts as 1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc */
#include <stdint.h>   /* for size_t, etc. */

/* We rely on the types seq_t (value type) and idx_t (index type) being
   defined by the build system.  No redefinition here. */

static seq_t *dp   = NULL;   /* dynamic DP table, persistent between calls */
static idx_t  dp_sz = 0;     /* current allocated size (dp_sz == n+1 when ready) */

/* returns the number of partitions of n into squares that do not divide n */
seq_t a(idx_t n)
{
    /* Ensure the table is large enough for indices up to n.  We
       keep the table persistent to avoid reallocation on subsequent
       calls. */
    if (dp_sz <= n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *p = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!p) { /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        dp = p;
        /* Initialize new slots to 0 */
        for (idx_t i = dp_sz; i <= n; ++i) {
            dp[i] = (seq_t)0;
        }
        dp_sz = new_sz;
    }

    if (n == 0) {
        /* a(0) = 1 by definition */
        dp[0] = (seq_t)1;
        return dp[0];
    }

    /* Initialize DP: only zero sum is reachable initially. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Iterate over all squares <= n that do not divide n */
    for (idx_t i = 1; ; ++i) {
        idx_t sq = i * i;
        if (sq > n)
            break;
        if (n % sq == 0)      /* skip squares that divide n */
            continue;

        /* Unbounded knapsack update for this square */
        for (idx_t sum = sq; sum <= n; ++sum) {
            dp[sum] += dp[sum - sq];
        }
    }

    return dp[n];
}
