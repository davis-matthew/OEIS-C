/*
 * A372893  Number of distinct partitions p of n such that max(p) == 0 mod 3.
 * https://oeis.org/A372893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A372893: the number of distinct partitions of n
   whose largest part is divisible by 3.  The computation uses a simple
   dynamic programming approach that counts partitions by distinct part
   sizes.  It works for all n ≥ 0.  The implementation is fully
   compatible with C11 and C++17. */

#include <stdlib.h>   /* malloc, free, memset */

seq_t a(idx_t n)
{
    /* Special case: the empty partition of 0 has largest part 0 ≡ 0 (mod 3) */
    if (n == 0)
        return (seq_t)1;

    /* Dynamically grow a shared DP array as large as necessary.
       dp[s*3 + r] counts partitions of sum s whose largest part ≡ r (mod 3). */
    static seq_t *dp = NULL;
    static idx_t cap = 0;            /* current capacity in slots (not bytes) */

    idx_t required = (n + 1) * 3;    /* number of slots needed */
    if (dp == NULL || required > cap) {
        seq_t *tmp = (seq_t *)realloc(dp, required * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;           /* allocation failed – return 0 */
        dp = tmp;
        cap = required;
    }

    /* Zero out the DP table up to the required size. */
    memset(dp, 0, required * sizeof(seq_t));
    dp[0] = (seq_t)1;                 /* dp[0][0] = 1 */

    /* Iterate over possible part sizes j from 1 to n.
       For each j, update all sums i ≥ j in decreasing order
       to maintain distinctness. */
    for (idx_t j = 1; j <= n; ++j) {
        idx_t r = j % 3;              /* remainder of this new part */
        for (idx_t i = n; i >= j; --i) {
            idx_t base = (i - j) * 3;
            dp[i * 3 + r] += dp[base + 0] + dp[base + 1] + dp[base + 2];
        }
    }

    /* The answer is the number of partitions of n with largest part ≡ 0 (mod 3). */
    return dp[n * 3 + 0];
}
