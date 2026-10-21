/*
 * A284837  Expansion of Sum_{i>=1} x^(i^3)/(1 - x^(i^3)) * Product_{j=1..i}
 *          1/(1 - x^(j^3)).
 * https://oeis.org/A284837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of largest parts in all partitions of n into
 *            cubes
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    /* static memoisation */
#ifdef __cplusplus
    static std::vector<seq_t> dp_static;
    static std::vector<seq_t> ans_static;
    static idx_t static_max_n = 0;
#else
    static seq_t *dp_static = NULL;
    static seq_t *ans_static = NULL;
    static idx_t static_max_n = 0;
#endif

    if (n <= static_max_n) {
        return ans_static[n];
    }

    /*--- compute cube list up to n -------------------------------------*/
    size_t cubes_cap = 16;
    size_t cubes_cnt = 0;
    idx_t *cubes = (idx_t*)malloc(cubes_cap * sizeof(idx_t));
    for (idx_t k = 1;; ++k) {
        idx_t cube = k * k * k;
        if (cube > n) break;
        if (cubes_cnt == cubes_cap) {
            cubes_cap <<= 1;
            cubes = (idx_t*)realloc(cubes, cubes_cap * sizeof(idx_t));
        }
        cubes[cubes_cnt++] = cube;
    }

    /*--- allocate temporary DP arrays ----------------------------------*/
#ifdef __cplusplus
    std::vector<seq_t> dp(n + 1, 0);
    std::vector<seq_t> ans(n + 1, 0);
#else
    seq_t *dp = (seq_t*)calloc(n + 1, sizeof(seq_t));
    seq_t *ans = (seq_t*)calloc(n + 1, sizeof(seq_t));
#endif
    dp[0] = 1;   /* empty partition */

    /*--- main DP loop over cubes -------------------------------------*/
    for (size_t ci = 0; ci < cubes_cnt; ++ci) {
        idx_t c = cubes[ci];

        /* contributions of partitions whose largest part is c */
        for (idx_t r = 0; r < c; ++r) {
            seq_t pref0 = dp[r];
            seq_t pref1 = 0;
            idx_t m = 0;
            for (idx_t s = r + c; s <= n; s += c) {
                ++m;
                seq_t add_val = (seq_t)m * pref0 - pref1;
                ans[s] += add_val;
                pref0 += dp[s];
                pref1 += (seq_t)m * dp[s];
            }
        }

        /* usual partition DP update for unbounded coin */
        for (idx_t s = c; s <= n; ++s) {
            dp[s] += dp[s - c];
        }
    }

    free(cubes);

    /*--- store results for future calls ------------------------------*/
#ifdef __cplusplus
    dp_static.assign(dp.begin(), dp.end());
    ans_static.assign(ans.begin(), ans.end());
    static_max_n = n;
#else
    if (dp_static) free(dp_static);
    if (ans_static) free(ans_static);
    dp_static = dp;
    ans_static = ans;
    static_max_n = n;
#endif

    return ans_static[n];
}
