/*
 * A298246  Expansion of Product_{k>=1} (1 + x^(k*(k+1)*(2*k+1)/6)).
 * https://oeis.org/A298246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct square pyramidal numbers
 *            (A298246), where the square pyramidal number for k is
 *            k*(k+1)*(2*k+1)/6. The algorithm uses a standard subset–sum
 *            dynamic programming that runs in O(K·n) time with K ≈ (3n)^(1/3)
 *            distinct pyramid numbers
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
    #include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate an array of size n+1.  Use new/delete for C++ (ensures
       proper construction of arbitrary‑precision seq_t types) and malloc
       for plain C. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1]();   /* value‑initialized to 0 */
#else
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                /* allocation failure */
    dp[0] = 1;                        /* base case */
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;
#endif
    /* Ensure dp[0] = 1 for both C and C++. */
    dp[0] = 1;

    /* Enumerate square pyramidal numbers up to n */
    for (idx_t k = 1; ; ++k) {
        seq_t sq = (seq_t)k * (k + 1) * (2 * k + 1) / 6;  /* k*(k+1)*(2k+1)/6 */
        idx_t s = (idx_t)sq;
        if (s > n) break;

        /* Subset‑sum DP update for distinct items */
        for (idx_t j = n; j >= s; --j) {
            dp[j] = dp[j] + dp[j - s];
        }
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
