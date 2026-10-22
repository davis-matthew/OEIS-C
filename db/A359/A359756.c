/*
 * A359756  First position of n in the sequence of zero-based weighted sums of
 *          standard compositions (A124757), if we start with position 0.
 * https://oeis.org/A359756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A359756: first position of n in the sequence of zero‑based weighted
    sums of standard compositions (A124757).
    Formula: a(n) = min{k >= 0 : weighted_sum(k) = n}
    where weighted_sum(k) = sum_{i=1}^{m} (i-1) * y_i
    for the m‑part composition corresponding to the binary representation of k.
    This implementation finds the minimal k by dynamic programming over
    the positions of 1‑bits of k. It runs in O(n·√n) time and uses O(n)
    memory, suitable for n up to about 10000. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* n == 0 occurs at position 0 */
    if (n == 0)
        return (seq_t)0;

    /* Determine the largest position that could contribute to a weighted
       sum of n.  We need 1+2+…+M-1 >= n. */
    idx_t pos_max = 0;
    while (pos_max * (pos_max + 1) / 2 < n)
        pos_max++;

    /* DP: dp[s] holds the minimal sum of 2^(p-1) over subsets of
       {1,…,current_pos} that reach total position sum s.  A separate
       boolean array marks which sums are reachable. */
    seq_t *dp    = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    bool   *reach = (bool   *)malloc((n + 1) * sizeof(bool));
    if (!dp || !reach) {
        if (dp) free(dp);
        if (reach) free(reach);
        abort();    /* allocation failed */
    }

    for (idx_t i = 0; i <= n; ++i)
        reach[i] = false;
    dp[0] = 0;
    reach[0] = true;

    bool found = false;
    seq_t best = 0;

    /* Iterate over possible highest bit position (M-1). */
    for (idx_t pos = 1; pos <= pos_max; ++pos) {
        seq_t add_bit = pow2(pos - 1);   /* weight contributed by this 1‑bit */

        /* Update DP for using this position. Iterate sums descending. */
        for (idx_t sum = n; sum >= pos; --sum) {
            if (!reach[sum - pos])
                continue;
            seq_t cand = dp[sum - pos] + add_bit;
            if (!reach[sum] || cand < dp[sum]) {
                dp[sum] = cand;
                reach[sum] = true;
            }
        }

        /* If we can achieve the target weighted sum n, compute the candidate
           position k: add the highest‑bit contribution 2^pos. */
        if (reach[n]) {
            seq_t candidate_k = dp[n] + pow2(pos);   /* 2^pos = 2^(M-1) */
            if (!found || candidate_k < best) {
                best = candidate_k;
                found = true;
            }
        }
    }

    free(dp);
    free(reach);

    return found ? best : (seq_t)0;   /* should always be true for n>0 */
}
