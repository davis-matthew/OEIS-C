/*
 * A280909  Expansion of Product_{k>=1} ((1-x^(12*k)) * (1-x^(12*k-10)) *
 *          (1-x^(12*k-9)) / (1-x^k)).
 * https://oeis.org/A280909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts whose residue * mod
 *            12 is not 0, 2, or 3. This follows from the generating *
 *            function ∏_{k≥1}((1−x^{12k})(1−x^{12k−10})(1−x^{12k−9})/(1−x^k))
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 178, u64 n <= 571,
 *            u128 n <= 1975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate workspace for dynamic programming.  dp[i] holds the
     * number of partitions of i using only allowed part sizes. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return 0;          /* allocation failure – best effort */

    dp[0] = (seq_t)1;      /* empty partition */

    for (idx_t part = 1; part <= n; ++part) {
        /* Skip forbidden residues mod 12: 0, 2, 3. */
        idx_t r = part % (idx_t)12;
        if (r == (idx_t)0 || r == (idx_t)2 || r == (idx_t)3)
            continue;

        /* Update DP for this part size.  Standard coin‑change
         * (unlimited) formulation: for each i ≥ part, add ways to
         * reach i−part. */
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
