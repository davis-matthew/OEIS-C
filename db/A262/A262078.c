/*
 * A262078  Number T(n,k) of partitions of an n-set with distinct block sizes
 *          and maximal block size equal to k; triangle T(n,k), k>=0,
 *          k<=n<=k*(k+1)/2, read by columns.
 * https://oeis.org/A262078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 38, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th entry of triangle T(n,k) read by columns.  
   For each column k we compute T(n,k)=C(n,k)*P(n-k,k-1) where  
   P(m,k-1) is the number of ways to partition an m‑set into distinct block sizes  
   drawn from {1,…,k-1}.  The function builds the triangle column by column,  
   storing the values in a cache that is indexed exactly as the OEIS a(n). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Cache the values of the sequence in the order they are produced. */
    static seq_t *cache     = NULL;   /* pointer to cached values */
    static size_t cache_size = 0;     /* number of values stored */
    static size_t cache_cap   = 0;     /* allocated capacity */
    static idx_t  next_k  = 0;        /* next column to compute */

    /* Produce columns until the cache contains the requested index. */
    while (cache_size <= n) {
        idx_t k = next_k;                /* current column index */

        /* Number of elements in this column:
           for k = 0 it is 1; for k ≥ 1 it is k(k−1)/2 + 1. */
        idx_t maxR = k * (k - 1) / 2;     /* maximum offset beyond k */

        /* Allocate DP array for the current column. */
#ifdef __cplusplus
        std::vector<seq_t> dp(maxR + 1);
#else
        seq_t *dp = (seq_t *)malloc((maxR + 1) * sizeof(seq_t));
        if (!dp) abort();
#endif
        /* Initialize DP: dp[0] = 1, others = 0. */
        dp[0] = (seq_t)1;
#ifdef __cplusplus
        for (idx_t r = 1; r <= maxR; ++r) dp[r] = (seq_t)0;
#else
        for (idx_t r = 1; r <= maxR; ++r) dp[r] = (seq_t)0;
#endif

        /* Dynamic programming over block sizes 1 .. k-1. */
        for (idx_t i = 1; i < k; ++i) {
            for (idx_t r = maxR; r >= i; --r) {
                seq_t choose = binom(r, i);
                dp[r] = dp[r] + dp[r - i] * choose;
            }
        }

        /* Append the computed column values to the cache. */
        for (idx_t R = 0; R <= maxR; ++R) {
            idx_t row = k + R;                     /* actual n value */
            seq_t bin = binom(row, k);              /* choose the max block */
            seq_t val = bin * dp[R];                /* total partitions */

            if (cache_size >= cache_cap) {          /* grow cache if needed */
                size_t new_cap = (cache_cap == 0) ? 4 : 2 * cache_cap;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }

#ifdef __cplusplus
        /* dp vector will be destroyed automatically. */
#else
        free(dp);
#endif
        ++next_k;                                  /* move to next column */
    }

    return cache[n];
}
