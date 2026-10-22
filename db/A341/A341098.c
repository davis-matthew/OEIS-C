/*
 * A341098  Number of partitions of n into 10 distinct squarefree parts.
 * https://oeis.org/A341098
 *
 * Offset:    72    (a(72) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 10 distinct squarefree
 *            parts
 * Range:     u8 n <= 102, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each iteration adds a new squarefree part to the subset-sum DP. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static state to allow memoization across calls */
    static idx_t current_limit = 0;          /* maximum n computed so far            */
    static seq_t **dp = NULL;                /* dp[k][s]: partitions with k parts   */
    static unsigned char *sqf = NULL;        /* squarefree indicator array           */

    if (n <= current_limit && dp != NULL)
        return dp[10][n];

    /* ------------------------------------------------------------------------- */
    /* Build new DP for all n up to 'limit', recomputed from scratch.             */
    /* ------------------------------------------------------------------------- */
    idx_t limit = n;
    if (limit < 1)
        limit = 1;           /* avoid zero-size arrays on very small inputs */

    /* Step 1: compute squarefree table. */
    unsigned char *new_sqf = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    /* mark all numbers as squarefree initially (cf. 1 is squarefree) */
    memset(new_sqf, 1, limit + 1);

    /* Mark numbers that are divisible by a square (>1).        */
    for (int64_t p = 2; p * p <= (int64_t)limit; ++p) {
        int64_t sq = p * p;
        for (int64_t j = sq; j <= (int64_t)limit; j += sq)
            new_sqf[j] = 0;
    }

    /* Step 2: allocate DP arrays. */
    seq_t **new_dp = (seq_t **)calloc(11, sizeof(seq_t *));
    for (int k = 0; k <= 10; ++k)
        new_dp[k] = (seq_t *)calloc(limit + 1, sizeof(seq_t));

    new_dp[0][0] = 1;   /* one way to partitition 0 with 0 parts */

    /* Step 3: populating the DP with each squarefree number. */
    for (idx_t x = 1; x <= limit; ++x) {
        if (!new_sqf[x])
            continue;   /* skip non‑squarefree number */
        for (int k = 10; k >= 1; --k) {
            for (idx_t s = limit; s >= x; --s)
                new_dp[k][s] += new_dp[k - 1][s - x];
        }
    }

    /* Step 4: free old resources and install new ones. */
    if (dp != NULL) {
        for (int k = 0; k <= 10; ++k)
            free(dp[k]);
        free(dp);
    }
    if (sqf != NULL)
        free(sqf);

    dp = new_dp;
    sqf = new_sqf;
    current_limit = limit;

    return dp[10][n];
}
