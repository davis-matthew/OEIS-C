/*
 * A347805  Expansion of (theta_3(x) - 1)^2 / (2 * (3 - theta_3(x))).
 * https://oeis.org/A347805
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into two or more positive
 *            squares
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = DP(n) - (n perfect square ? 1 : 0), where
   DP(0) = 1, DP(i) = Σ_{s^2 ≤ i} DP(i - s^2). */

#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Check whether n is a perfect square (simple O(√n) test). */
static inline bool is_square(idx_t n)
{
    for (idx_t s = 1; s * s <= n; ++s)
        if (s * s == n)
            return true;
    return false;
}

/* Return a(n) for n ≥ 2.  Values for n < 2 are undefined and 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;        /* out of defined range */

    /* Static DP table.  dp[i] holds the number of compositions of i into squares. */
    static seq_t *dp = NULL;      /* dp[0 .. dp_len-1] */
    static idx_t dp_len = 0;      /* number of valid entries currently stored */

    /* First use: allocate dp[0] and set base case. */
    if (dp_len == 0) {
        dp = seq_alloc(1);           /* allocate room for at least dp[0] */
        if (!dp)
            return (seq_t)0;         /* allocation failure */
        dp[0] = (seq_t)1;            /* empty composition */
        dp_len = 1;
    }

    /* Extend the table up to index n. */
    if (n >= dp_len) {
        seq_t *tmp = seq_realloc(dp, dp_len, n + 1);
        if (!tmp)
            return (seq_t)0;         /* allocation failure */
        dp = tmp;
        /* Compute new entries one by one. */
        for (idx_t i = dp_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t s = 1; s * s <= i; ++s) {
                idx_t sub = i - s * s;
                sum += dp[sub];
            }
            dp[i] = sum;
        }
        dp_len = n + 1;
    }

    seq_t result = dp[n];
    if (is_square(n))
        result -= (seq_t)1;    /* remove the single‑part composition if n is a square */
    return result;
}
