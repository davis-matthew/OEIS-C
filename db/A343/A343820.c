/*
 * A343820  Number of partitions of 2n into powers of 2: p1 <= p2 <= ... <=
 *          p_k such that p_i <= 1 + Sum_{j=1..i-1} p_j.
 * https://oeis.org/A343820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 79, u32 n <= 562, u64 n <= 7864,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of 2n into powers of 2 with
 * nondecreasing parts p1 ≤ p2 ≤ … ≤ pk such that for every i
 *     p_i ≤ 1 + Σ_{j=1}^{i-1} p_j
 *
 * The implementation builds a dynamic programming table
 * dp[s][k] = number of valid sequences summing to s
 * whose last part is 2^k.
 * The recurrence is
 *     dp[s][k] = Σ_{p≤k} dp[s - 2^k][p]          if s ≥ 2^(k+1) - 1
 * and the only base case is dp[1][0] = 1 (single part 1).
 *
 * Finally a(n) = Σ_k dp[2n][k].
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    idx_t targetS = n << 1;               /* 2n */

    /* compute number of possible exponents for powers of 2 up to targetS */
    int expCount = 0;
    idx_t tmp = targetS;
    do { expCount++; tmp >>= 1; } while (tmp);

    /* allocate DP table: (targetS+1) rows × expCount columns */
    seq_t *dp = (seq_t *)calloc((targetS + 1) * expCount, sizeof(seq_t));
    if (!dp) return (seq_t)0; /* allocation failure, return 0 */

    /* base case: single part 1 */
    if (targetS >= 1) dp[1 * expCount + 0] = (seq_t)1;

    /* DP iteration */
    for (idx_t s = 2; s <= targetS; ++s) {
        for (int k = 0; k < expCount; ++k) {
            seq_t val = ((seq_t)1) << k;         /* 2^k */
            if (val > s) break;                 /* larger powers useless */

            seq_t threshold = (((seq_t)1) << (k + 1)) - 1;
            if (s < threshold) continue;        /* condition s ≥ 2^(k+1) - 1 */

            idx_t prev = s - (idx_t)val;
            seq_t sum = 0;
            for (int pk = 0; pk <= k; ++pk) {
                sum += dp[prev * expCount + pk];
            }
            dp[s * expCount + k] = sum;
        }
    }

    /* compute result by summing over last part exponents */
    seq_t result = 0;
    for (int k = 0; k < expCount; ++k) {
        result += dp[targetS * expCount + k];
    }

    free(dp);
    return result;
}
