/*
 * A331128  Number of ways to write n as n = h_1*1! + h_2*2! + ... + h_k*k!
 *          where 0 <= h_i <= 2*i for all i.
 * https://oeis.org/A331128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence A331128 counts the number of ways to write an integer n as
 * a sum n = Σ h_i * i!  with 0 ≤ h_i ≤ 2*i.  
 * We compute it by a bounded‑knapsack dynamic programming over factorials.
 *
 * Algorithm: 
 *   dp[s] = number of ways to represent s using factorials up to i-1.
 *   For each factorial i! up to n, we form a new array newdp where
 *   newdp[t] += dp[s] for all s and coefficients c ∈ [0, 2*i] with
 *   t = s + c * i!.  After processing i, we replace dp by newdp.
 *   The result is dp[n].
 *
 * Complexity: For n ≤ 10 000 the number of factorials involved is ≤ 7
 * (since 8! > 10 000).  The work is thus O(n × Σ(2*i)) ≈ 1 million
 * elementary operations, well below the time limit.
 */

#ifdef __cplusplus
#   include <vector>
#   include <cstdint>
#else
#   include <stdlib.h>
#   include <stdint.h>
#endif

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;
#ifndef __cplusplus
    /* ---------- C implementation ---------- */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newdp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) { free(dp); free(newdp); return (seq_t)0; }
    dp[0] = (seq_t)1;
    idx_t fact = 1;
    /* process each factorial i! up to n */
    for (idx_t i = 1; ; ++i) {
        fact = fact * i;                      /* i! */
        if (fact > n) break;
        int max_coef = (int)(2 * i);
        /* zero newdp */
        for (size_t k = 0; k < sz; ++k) newdp[k] = (seq_t)0;
        for (idx_t s = 0; s <= n; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            idx_t t = s;
            for (int c = 0; c <= max_coef; ++c, t += fact) {
                if (t > n) break;
                newdp[t] = newdp[t] + ways;      /* integer addition */
            }
        }
        /* swap dp and newdp */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }
    seq_t result = dp[(size_t)n];
    free(dp); free(newdp);
    return result;
#else
    /* ---------- C++ implementation (allows big‑int seq_t) ---------- */
    std::vector<seq_t> dp((size_t)n + 1, seq_t(0));
    std::vector<seq_t> newdp(dp.size());
    dp[0] = seq_t(1);
    idx_t fact = 1;
    for (idx_t i = 1; ; ++i) {
        fact = fact * i;                        /* i! */
        if (fact > n) break;
        int max_coef = (int)(2 * i);
        std::fill(newdp.begin(), newdp.end(), seq_t(0));
        for (idx_t s = 0; s <= n; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            idx_t t = s;
            for (int c = 0; c <= max_coef; ++c, t += fact) {
                if (t > n) break;
                newdp[t] = newdp[t] + ways;
            }
        }
        dp.swap(newdp);
    }
    return dp[(size_t)n];
#endif
}
