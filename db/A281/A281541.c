/*
 * A281541  Expansion of Sum_{i>=1} x^(i^2)/(1 - x^(i^2)) / Product_{j>=1} (1
 *          - x^(j^2)).
 * https://oeis.org/A281541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into squares
 *            (equivalently a(n) = Sum_{k=1..n} k * A243148(n,k) )
 * Range:     u8 n <= 27, u16 n <= 115, u32 n <= 602, u64 n <= 3661,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* a(n) = total number of parts in all partitions of n into squares. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* Allocate arrays for counts and total parts */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *tot = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !tot)
        return 0; /* allocation failure */

    dp[0] = 1;
    tot[0] = 0;

    /* Coin‑change DP: use each square size as an unlimited coin */
    for (idx_t s = 1; s * s <= n; ++s) {
        idx_t square = s * s;
        for (idx_t j = square; j <= n; ++j) {
            seq_t add_part = dp[j - square];
            dp[j] = dp[j] + add_part;                  /* new partitions */
            tot[j] = tot[j] + tot[j - square] + add_part; /* total parts */
        }
    }

    seq_t result = tot[n];
    free(dp);
    free(tot);
    return result;
}
