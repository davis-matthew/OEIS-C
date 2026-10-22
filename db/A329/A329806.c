/*
 * A329806  Expansion of Product_{k>=1} 1 / (1 - 6*x^k + x^(2*k))^(1/2).
 * https://oeis.org/A329806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1} 1 / sqrt(1 - 6*x^k +
 *            x^{2k})
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Allocate working arrays. Use new in C++ (value‑initialised), malloc/calloc in C. */
#ifdef __cplusplus
    size_t N = (size_t)n;
    seq_t *D   = new seq_t[N + 1]();   // Central Delannoy numbers
    seq_t *dp  = new seq_t[N + 1]();   // dp array for the product
    seq_t *tmp = new seq_t[N + 1]();   // temporary array
#else
    size_t N = (size_t)n;
    seq_t *D   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *tmp = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif

    /* Compute central Delannoy numbers D[0..N] */
    D[0] = (seq_t)1;
    if (N >= 1) D[1] = (seq_t)3;
    for (idx_t i = 1; i < N; ++i) {
        seq_t num = ((seq_t)(6 * i + 3)) * D[i] - ((seq_t)i) * D[i - 1];
        D[i + 1] = num / (seq_t)(i + 1);
    }

    /* Initial dp[0] = 1 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    /* Dynamic programming for the product */
    for (idx_t k = 1; k <= N; ++k) {
        /* Zero temporary array */
        for (idx_t i = 0; i <= N; ++i) tmp[i] = (seq_t)0;

        for (idx_t s = 0; s <= N; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;

            /* m = 0 term (D[0] = 1) */
            tmp[s] += val;

            /* m >= 1 terms */
            idx_t t = s + k;
            idx_t m = 1;
            while (t <= N) {
                tmp[t] += val * D[m];
                t += k;
                ++m;
            }
        }
        /* Swap dp and tmp pointers for next iteration */
        seq_t *tmpPtr = dp;
        dp = tmp;
        tmp = tmpPtr;
    }

    seq_t result = dp[N];

    /* Free allocated memory */
#ifdef __cplusplus
    delete[] D;
    delete[] dp;
    delete[] tmp;
#else
    free(D);
    free(dp);
    free(tmp);
#endif

    return result;
}
