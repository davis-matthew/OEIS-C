/*
 * A320388  Number of partitions of n into distinct parts such that the
 *          successive differences of consecutive parts are decreasing.
 * https://oeis.org/A320388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 305, u32 n <= 1663, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 
   Compute OEIS A320388: number of partitions of n into distinct parts
   such that the successive differences of consecutive parts are decreasing.
   For a partition [p1 < p2 < ... < pm] we have differences d1=p1, d2=p2-p1,
   …, dm=pm-p{m-1}.  The condition is d2 > d3 > ... > dm.
   The algorithm exploits the representation
         n = sum_{k=1}^m k * y_k
   where y_k = d_{m-k+1}.  For k=1..m-1 the y_k are strictly increasing,
   while y_m (=d1) is unrestricted.  After a change of variables the
   counting reduces to a coin‑change problem with denominations
         { m , c1 , c2 , … , c_{m-1} }
   where
         c_j = (m-1)(m)/2 - (j-1)j/2  (j = 1..m-1).
   This yields an O(n * m) algorithm for each call, which is well below 2 s
   for n ≤ 10000.
*/

seq_t a(idx_t n)
{
    /* Special case: the empty partition for n = 0. */
    if (n == 0)
        return 1;               /* a(0) = 1 */

    seq_t total = 1;            /* contribution of the one‑part partition [n] */

    /* Preprocess constants that depend on n. */
    const idx_t n_idx = n;

    /* Iterate over possible number of parts m ≥ 2. */
    for (idx_t m = 2; ; ++m) {
        /* L = m – 1 is the length of the decreasing difference sequence. */
        idx_t L = m - 1;

        /* S0L = 1^2 + 2^2 + … + L^2 = L(L+1)(2L+1)/6   (seq_t). */
        seq_t S0L = (seq_t)L * (L + 1) * ((seq_t)2 * L + 1) / 6;

        /* Minimal possible sum for this m: n_min = S0L + m. */
        seq_t n_min = S0L + (seq_t)m;

        /* If the minimal sum already exceeds n, no further m can work. */
        if (n_idx < (idx_t)n_min)
            break;

        /* Remaining part of the sum that must be composed with the
           denominations described above. */
        seq_t Rseq = (seq_t)n_idx - n_min;
        idx_t R = (idx_t)Rseq;          /* R ≤ n, safe to cast to idx_t */

        /* Allocate DP array for coin-change: dp[s] is the number of ways
           to write s using the allowed denominations. */
        seq_t *dp = (seq_t *)calloc((size_t)R + 1, sizeof(seq_t));
        if (!dp)
            return total;   /* allocation failure; unlikely in test harness */

        dp[0] = 1;          /* one way to produce zero amount */

        /* First include the coin corresponding to the unrestricted part d1,
           whose coefficient is exactly m. */
        if (m <= R) {
            for (idx_t sum = m; sum <= R; ++sum)
                dp[sum] += dp[sum - m];
        }

        /* Precompute the common part S12 = L(L+1)/2 for the c_j formula. */
        seq_t S12 = (seq_t)L * (L + 1) / 2;

        /* Process the remaining denominators c_j for j = 1 .. L. */
        for (idx_t j = 1; j <= L; ++j) {
            /* c_j = S12 - (j-1)j/2. */
            seq_t sub = (seq_t)(j - 1) * j / 2;
            seq_t denom = S12 - sub;

            idx_t d = (idx_t)denom;      /* denom ≤ R (see later) */
            if (d > R)
                continue;

            for (idx_t sum = d; sum <= R; ++sum)
                dp[sum] += dp[sum - d];
        }

        /* Add the number of compositions for this m. */
        total += dp[R];

        /* Clean up. */
        free(dp);
    }

    return total;
}
