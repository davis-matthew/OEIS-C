/*
 * A375134  Number of integer partitions of n whose maximal anti-runs have
 *          distinct minima.
 * https://oeis.org/A375134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 185, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * formula:  a(n) = sum_{m=1}^{n} p_{≤2}(n-m, min=m+1)  (partitions of n-m
 * into parts ≥ m+1, each part used at most twice).  The empty partition
 * contributes a(0)=1.  This implementation builds the
 * generating function ∏_{j≥1}(1+x^j+x^{2j}) and, for every lower bound
 * m+1, it adds the corresponding terms to the result.
 */

#include <stdlib.h>

/* external types supplied by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* handle offset n = 0 directly */
    if (n == 0)
        return 1;

    /* allocate DP arrays and result array */
    size_t len = (size_t)(n + 1);
    seq_t *dp      = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *dp_new  = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *result  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp || !dp_new || !result) {
        /* memory failure – abort (not expected in test environment) */
        exit(EXIT_FAILURE);
    }

    dp[0] = 1;          /* one way to partition 0 */

    /* iterate over possible part sizes j from n down to 1 */
    for (idx_t j = n; j >= 1; --j) {
        /* update dp to include part size j with multiplicity 0,1,2 */
        for (idx_t s = 0; s <= n; ++s) {
            seq_t val = dp[(size_t)s];
            if (s >= j)
                val += dp[(size_t)(s - j)];
            if (s >= 2 * j)
                val += dp[(size_t)(s - 2 * j)];
            dp_new[(size_t)s] = val;
        }

        /* swap dp and dp_new for next iteration */
        seq_t *tmp = dp;
        dp = dp_new;
        dp_new = tmp;

        /* for minimal part m = j-1, add contributions to result */
        idx_t m = j - 1;
        if (m >= 1) {
            /* dp contains counts for partitions with parts ≥ m+1 */
            for (idx_t d = 0; d <= n - m; ++d) {
                result[(size_t)(d + m)] += dp[(size_t)d];
            }
        }
    }

    /* add the case where the minimal part equals n (single part n) */
    result[(size_t)n] += 1;

    /* result[0] is already zero; set a(0)=1 explicitly */
    result[0] = 1;

    seq_t ans = result[(size_t)n];

    free(dp);
    free(dp_new);
    free(result);

    return ans;
}
