/*
 * A332005  Number of compositions (ordered partitions) of n into distinct
 *          centered triangular numbers.
 * https://oeis.org/A332005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ over subsets S of distinct centered triangular numbers
 *            with ΣS = n of |S|! . The centered triangular numbers are C_k =
 *            3k(k-1)/2 + 1, for k = 1,2,3,…
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count compositions of n into distinct centered triangular numbers.
   The algorithm uses dynamic programming over subsets:
   dp[sum][k] = number of subsets of size k whose elements sum to sum.
   After filling dp, a(n) = Σ_k dp[n][k] * k! . */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* By definition a(0) = 1 (empty composition). */
    if (n == 0)
        return (seq_t)1;

    /* Use int for loop counters – n is at most ~10000 in the tests. */
    int N = (int)n;

    /* Build list of centered triangular numbers ≤ N. */
    int ct_vals[200];
    int ct_count = 0;
    for (int k = 1; ; ++k) {
        long long ct = (3LL * k * (k - 1)) / 2 + 1;  /* CT_k */
        if (ct > N)
            break;
        ct_vals[ct_count++] = (int)ct;
        if (ct_count == 200)  /* safeguard; should never hit for N≈10000 */
            break;
    }

    /* DP table dimensions */
    size_t rows = (size_t)(N + 1);
    size_t cols = (size_t)(ct_count + 1);
    seq_t *dp = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* out of memory – return 0 as safe value */

    /* Empty subset for sum 0 */
    dp[0] = (seq_t)1;

    /* Populate DP */
    for (int idx = 0; idx < ct_count; ++idx) {
        int val = ct_vals[idx];
        for (int sum = N; sum >= val; --sum) {
            size_t base_cur  = (size_t)sum * cols;
            size_t base_prev = (size_t)(sum - val) * cols;
            for (int sz = 1; sz <= ct_count; ++sz) {
                seq_t add = dp[base_prev + sz - 1];
                if (add != (seq_t)0)
                    dp[base_cur + sz] += add;
            }
        }
    }

    /* Precompute factorials up to ct_count */
    seq_t *fact = (seq_t*)calloc(ct_count + 1, sizeof(seq_t));
    if (!fact) {
        free(dp);
        return (seq_t)0;
    }
    fact[0] = (seq_t)1;
    for (int i = 1; i <= ct_count; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Sum contributions for sum = N */
    seq_t result = (seq_t)0;
    size_t base = (size_t)N * cols;
    for (int sz = 1; sz <= ct_count; ++sz) {
        seq_t cnt = dp[base + sz];
        if (cnt != (seq_t)0)
            result += cnt * fact[sz];
    }

    free(dp);
    free(fact);
    return result;
}
