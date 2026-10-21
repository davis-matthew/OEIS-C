/*
 * A284464  Number of compositions (ordered partitions) of n into squarefree
 *          divisors of n.
 * https://oeis.org/A284464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into squarefree divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 41, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   dfs_squarefree, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"        /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/dfs_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* factor n and collect distinct primes */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t k = 0;
    factor_u64((uint64_t)n, primes, exps, &k);

    /* generate all square‑free divisors of n */
    idx_t sq_divs[128];
    size_t sq_cnt = 0;
    dfs_squarefree(0, k, (idx_t)1, primes, sq_divs, &sq_cnt);

    /* DP array for compositions up to n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;             /* allocation failed, return 0 */

    dp[0] = (seq_t)1;
    for (idx_t s = 1; s <= n; ++s) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < sq_cnt; ++j) {
            idx_t d = sq_divs[j];
            if (d <= s) sum += dp[s - d];
        }
        dp[s] = sum;
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
